#include "matrix4.h"

class Shader{

protected:

    Matrix4 viewport;
    Matrix4 projection;
    Matrix4 modelView;
    Matrix4 pipeline;


public:


    Vertex light_source;


    Shader(const Matrix4 &viewport, const Matrix4 &projection, const Matrix4& modelView, const Vertex& light_source):
        modelView(modelView),
        viewport(viewport),
        projection(projection),
        light_source(light_source)
    {
        updatePipeline();
    }

    void setProjection(const Matrix4 &m){projection = m; updatePipeline();}
    void setViewport(const Matrix4 &m){viewport = m; updatePipeline();}
    void setWorld(const Matrix4 &m){modelView = m; updatePipeline();}

    virtual void updatePipeline(){
        pipeline = viewport * projection * modelView ;
    }

    virtual Vertex vertex( const Vertex &v){
      return pipeline * v;
    }

    virtual bool fragment(const Vertex &bary, TGAColor &c) = 0;

};

class Gouraud : public Shader{


public:


    Triangle normals;

    Gouraud(const Matrix4& world, const Matrix4 &viewport, const Matrix4 &projection, const Triangle &normals, const Vertex& light_source):
        Shader(world, viewport, projection, light_source),
        normals(normals)
        {}

    virtual bool fragment(const Vertex &bary, TGAColor &c){
        Vertex n_gouraud = (*normals.v1 * bary.x + *normals.v2 * bary.y + *normals.v3 * bary.z).normalized() * light_source;
        float gouraud_light = max( n_gouraud.x  + n_gouraud.y + n_gouraud.z , 0.f);
        c = TGAColor (min( (int) ( c.r * gouraud_light), 255),
                      min( (int) ( c.g * gouraud_light), 255),
                      min( (int) ( c.b * gouraud_light), 255),
                      c.a);
        return false;
    }



};


class RealisticShader : public Shader{


public:

    TGAImage normal_map;
    TGAImage specular_map;
    Matrix4 projM;
    Matrix4 projMTI;

    RealisticShader(const Matrix4 &viewport, const Matrix4 &projection, Matrix4& modelView, const Vertex& light_source, const TGAImage &normal_map, const TGAImage &specular_map):
        Shader(viewport, projection, modelView, light_source),
        normal_map(normal_map),
        specular_map(specular_map)
    {
        updatePipeline();
    }


    virtual void updatePipeline(){
        Shader::updatePipeline();
        projM = projection * modelView;
        projMTI = projM.invert_transpose();
    }

    double x, y;
    virtual bool fragment(const Vertex &bary,TGAColor &c){

        TGAColor normal_color  = normal_map.get( x * normal_map.get_width(), y * normal_map.get_height() );
        Vertex n_map = (projM * ( Vertex( normal_color.b, normal_color.r, normal_color.g) )).normalized();
        Vertex l = (projMTI * light_source).normalized();
        Vertex reflected =  (n_map*l*2 - l).normalized();

        float diffuse_light  = max( n_map.x * l.x +     n_map.y * l.y +     n_map.z * l.z, 0.f);
        auto col = specular_map.get(x * specular_map.get_width(), y * specular_map.get_height()).b;

        float specular_light = max( pow( max(reflected.z, 0.0f), 10+col), 0.);

        float ambient_light  = 2;//5;


        float light = diffuse_light + specular_light;

        c = TGAColor (  min( (int) ( ambient_light + c.r * light), 255),
                        min( (int) ( ambient_light + c.g * light), 255),
                        min( (int) ( ambient_light + c.b * light), 255),
                        c.a);
        return false;
    }


};


class Metal : public RealisticShader{


public:

    TGAImage normal_map;
    TGAImage specular_map;
    Matrix4 projM;
    Matrix4 projMTI;

    Metal(const Matrix4 &viewport, const Matrix4 &projection, Matrix4& modelView, const Vertex& light_source, const TGAImage &normal_map):
        RealisticShader(viewport, projection, modelView, light_source, normal_map, TGAImage())
    {
        updatePipeline();
    }


    virtual bool fragment(const Vertex &bary,TGAColor &c){

        TGAColor normal_color  = normal_map.get( x * normal_map.get_width(), y * normal_map.get_height() );
        Vertex n_map = (projMTI * ( Vertex( normal_color.b, normal_color.r, normal_color.g) )).normalized();
        Vertex l = (projM * light_source).normalized();
        Vertex reflected =  (n_map*l*2 - l).normalized();

        float diffuse_light  = max( n_map.x * l.x +     n_map.y * l.y +     n_map.z * l.z, 0.f);
        auto col = 50;

        float specular_light = max( pow( max(reflected.z, 0.0f), 10+col), 0.);

        float ambient_light  = 2;//5;


        float light = diffuse_light + specular_light;

        c = TGAColor (  min( (int) ( ambient_light + c.r * light), 255),
                        min( (int) ( ambient_light + c.g * light), 255),
                        min( (int) ( ambient_light + c.b * light), 255),
                        c.a);
        return false;
    }
};


class Contour : public Shader{


public:

    Contour(const Matrix4 &viewport, const Matrix4 &projection, Matrix4& modelView):
        Shader(viewport, projection, modelView, Vertex()){}



    virtual bool fragment(const Vertex &bary, TGAColor &c){
        if(   (-0.1 < bary.x && bary.x < 0.1)
           || (-0.1 < bary.y && bary.z < 0.1)
           || (-0.1 < bary.z && bary.z < 0.1)){
            c = TGAColor(200,0,0, c.a);
            return false;
        }
        return true;
    }

};

class DominantColor : public Shader{

public:
    DominantColor(const Matrix4 &viewport, const Matrix4 &projection, Matrix4& modelView):
        Shader(viewport, projection, modelView, Vertex()){}



    virtual bool fragment(const Vertex &bary, TGAColor &c){
        double cDom = 1.1, cO = 0.9;
        unsigned char m = max(max(c.r, c.g), c.b);
        c = TGAColor(c.r == m ? c.r*cDom : c.r*cO, c.g== m ? c.g*cDom : c.g*cO, c.b == m ? c.b*cDom : c.b*cO, c.a);
        return false;
    }

};

class Cartoon : public Gouraud{

public:


    Cartoon(const Matrix4 &viewport, const Matrix4 &projection, Matrix4& modelView,const Triangle &normals,const Vertex & light_source):
        Gouraud(viewport, projection, modelView, normals, light_source){}



    double step(double it){
        for(double i = 1 ; i >= 0 ; i -= 0.2){
            if(i <= it){
                return i;
            }
        }
        return 0;
    }

    virtual bool fragment(const Vertex &bary, TGAColor &c){
        Vertex n_gouraud = (*normals.v1 * bary.x + *normals.v2 * bary.y + *normals.v3 * bary.z).normalized() * light_source;
        float gouraud_light = step(max( n_gouraud.x  + n_gouraud.y + n_gouraud.z , 0.f));
        c = TGAColor (min( (int) ( gouraud_light*c.r), 255),
                      min( (int) ( gouraud_light*c.g), 255),
                      min( (int) ( gouraud_light*c.b), 255),
                      c.a);
        return false;
    }

};

class BlackAndWhite : public Shader{


public:
    BlackAndWhite(const Matrix4 &viewport, const Matrix4 &projection, Matrix4& modelView):
        Shader(viewport, projection, modelView, Vertex()){}



    virtual bool fragment(const Vertex &bary, TGAColor &c){
        int m = (c.r + c.g + c.b)/3;
        c = TGAColor(m,m,m, c.a);
        return false;
    }
};
