//******************************************************************
//
//		Fractale
//
//*******************************************************************

#version 400

layout (location=0) out vec4 outColor;

uniform double zoom;	//Zoom
uniform double xpos;	//Position sur l'image
uniform double ypos;
uniform double puissance;
uniform dvec2 juliaC;
uniform float alpha;	//Angle de rotation
uniform  vec2 ecranDim;	//Dimensions de l'écran
uniform int iter;

int mandelbrot()
{
        double c=cos(alpha);
        double s=sin(alpha);

        double x=(gl_FragCoord.x-(ecranDim.x/2.));
        double y=(gl_FragCoord.y-(ecranDim.y/2.));

        double xr=x*c-y*s;
        double yr=x*s+y*c;

        double Cr=xr*zoom+xpos;
        double Ci=yr*zoom+ypos;

        double Zr=0.;
        double Zi=0.;
        double tmp;

        int i=0;

        while((Zr*Zr+Zi*Zi)<4. && i<iter)
        {
                tmp=Zr*Zr-Zi*Zi+Cr;
                Zi=2.*Zr*Zi+Ci;
                Zr=tmp;
                i++;
        }
        return i;

}

float mandelbrot_distance()
{
        double c=cos(alpha);
        double s=sin(alpha);

        double x=(gl_FragCoord.x-(ecranDim.x/2.));
        double y=(gl_FragCoord.y-(ecranDim.y/2.));

        double xr=x*c-y*s;
        double yr=x*s+y*c;

        float Cr=float(xr)*float(zoom)+float(xpos);
        float Ci=float(yr)*float(zoom)+float(ypos);

        float Zr=0.;
        float Zi=0.;
        float tmp;

        float Zpr=0.;
        float Zpi=0.;

        int i=0;

        float d=0.;
        float n;

        while((Zr*Zr+Zi*Zi)<1000. && i<iter)
        {
                tmp=2.*(Zr*Zpr-Zi*Zpi)+1.;
                Zpi=2.*(Zr*Zpi+Zi*Zpr);
                Zpr=tmp;

                tmp=Zr*Zr-Zi*Zi+Cr;
                Zi=2.*Zr*Zi+Ci;
                Zr=tmp;

                i++;
        }

        if(i<iter)
        {
            n=sqrt(Zr*Zr+Zi*Zi);
            d=n*log(n)/sqrt(Zpi*Zpi+Zpr*Zpr);
        }
        return d;

}

int mandelbrot3()
{
        double c=cos(alpha);
        double s=sin(alpha);

        double x=(gl_FragCoord.x-(ecranDim.x/2.));
        double y=(gl_FragCoord.y-(ecranDim.y/2.));

        double xr=x*c-y*s;
        double yr=x*s+y*c;

        double Cr=xr*zoom+xpos;
        double Ci=yr*zoom+ypos;

        double Zr=0.;
        double Zi=0.;
        double tmp;

        int i=0;

        while((Zr*Zr+Zi*Zi)<4. && i<iter)
        {
                tmp=Zr*Zr*Zr-Zi*Zi*3.*Zr+Cr;
                Zi=3.*Zr*Zr*Zi-Zi*Zi*Zi+Ci;
                Zr=tmp;
                i++;
        }
        return i;

}

int mandelbrot4()
{
        double c=cos(alpha);
        double s=sin(alpha);

        double x=(gl_FragCoord.x-(ecranDim.x/2.));
        double y=(gl_FragCoord.y-(ecranDim.y/2.));

        double xr=x*c-y*s;
        double yr=x*s+y*c;

        double Cr=xr*zoom+xpos;
        double Ci=yr*zoom+ypos;

        double Zr=0.;
        double Zi=0.;
        double tmp;

        int i=0;

        while((Zr*Zr+Zi*Zi)<4. && i<iter)
        {
                tmp=Zr*Zr*Zr*Zr-6.*Zr*Zr*Zi*Zi+Zi*Zi*Zi*Zi+Cr;
                Zi=4.*(Zr*Zr*Zr*Zi-Zi*Zi*Zi*Zr)+Ci;
                Zr=tmp;
                i++;
        }
        return i;

}

#define PI 3.141592635

int mandelbrot_pow()
{
        float c=cos(alpha);
        float s=sin(alpha);

        float x=(gl_FragCoord.x-(ecranDim.x/2.));
        float y=(gl_FragCoord.y-(ecranDim.y/2.));

        float xr=x*c-y*s;
        float yr=x*s+y*c;

        float Cr=xr*float(zoom)+float(xpos);
        float Ci=yr*float(zoom)+float(ypos);

        float Zr=0.;
        float Zi=0.;

        float cZr,sZi;

        float rho=0.;
        float theta=0.;
        float rho_temp;

        float rhoC=sqrt(Cr*Cr+Ci*Ci);
        float thetaC;


        if(Cr>0. && Ci >=0.) thetaC=atan(Ci/Cr);
        else if(Cr>0. && Ci <0.) thetaC=atan(Ci/Cr)+2.*PI;
        else if(Cr<0.) thetaC=atan(Ci/Cr)+PI;
        else if(Cr==0. && Ci>0.) thetaC=PI/2.;
        else if(Cr==0. && Ci<0.) thetaC=3.*PI/2.;

        float rhoC_carre=rhoC*rhoC;
        float rhoCsin=rhoC*sin(thetaC);
        float rhoCcos=rhoC*cos(thetaC);

        int i=0;

        float p=float(puissance);



        while((Zr*Zr+Zi*Zi)<4. && i<iter)
        {
                /*
                rho=pow(rho,p);
                theta*=p;

                rho_temp=sqrt(rho*rho+rhoC_carre+2.*rho*rhoC*cos(theta-thetaC));
                theta=atan( (rho*sin(theta)+rhoCsin) / (rho*cos(theta)+rhoCcos) );
                rho=rho_temp;
                */

                rho=sqrt(Zr*Zr+Zi*Zi);
                if(rho==0.) theta=0.;
                else
                {
                    theta=acos(Zr/rho);
                    if(Zi<0.)theta=-theta;
                }


                rho=pow(rho,p);
                theta*=p;

                Zr=rho*cos(theta)+Cr;
                Zi=rho*sin(theta)+Ci;


                i++;
        }
        return i;

}

int julia_pow()
{
        float c=cos(alpha);
        float s=sin(alpha);

        float x=(gl_FragCoord.x-(ecranDim.x/2.));
        float y=(gl_FragCoord.y-(ecranDim.y/2.));

        float xr=x*c-y*s;
        float yr=x*s+y*c;

        float Zr=xr*float(zoom)+float(xpos);
        float Zi=yr*float(zoom)+float(ypos);
        float rho,theta;

        int i=0;
        float p=float(puissance);
        float Cr=float(juliaC.x);
        float Ci=float(juliaC.y);

        while((Zr*Zr+Zi*Zi)<4. && i<iter)
        {
                rho=sqrt(Zr*Zr+Zi*Zi);
                if(rho==0.) theta=0.;
                else
                {
                    theta=acos(Zr/rho);
                    if(Zi<0.)theta=-theta;
                }


                rho=pow(rho,p);
                theta*=p;

                Zr=rho*cos(theta)+Cr;
                Zi=rho*sin(theta)+Ci;


                i++;
        }

        return i;

}

int julia()
{
        double c=cos(alpha);
        double s=sin(alpha);

        double x=(gl_FragCoord.x-(ecranDim.x/2.));
        double y=(gl_FragCoord.y-(ecranDim.y/2.));

        double xr=x*c-y*s;
        double yr=x*s+y*c;

        double Zr=xr*zoom+xpos;
        double Zi=yr*zoom+ypos;
        double tmp;

        int i=0;

        while((Zr*Zr+Zi*Zi)<2. && i<iter)
        {
                tmp=Zr*Zr-Zi*Zi+juliaC.x;
                Zi=2.*Zr*Zi+juliaC.y;
                Zr=tmp;
                i++;
        }
        return i;

}

void main( void )
{

        //int i=mandelbrot();
        float d=mandelbrot_distance();

        vec3 couleur;

        /*
        if(i==iter)
		{
			couleur=vec3(0.,0.,0.);
		}
	else
		{*/
		    //float f=float(i);
		    float f=d*10.;
		    //couleur=vec3(mod(f*0.11357,1.),mod(f*.093,1.),mod(f*0.012,1.));


            float disp_d=float(puissance)/1000.;

			if(d<disp_d && d>disp_d-0.00002)couleur=vec3(0.,0.,1.);
			//else if (d==0.)couleur=vec3(1.,0.,0.);

			else if(d>0.) couleur=vec3(1.-f,1.-f*10.,1.-f*100.);
			else couleur=vec3(0.,0.,0.);
		//}

        outColor = vec4(couleur,1.);
}
