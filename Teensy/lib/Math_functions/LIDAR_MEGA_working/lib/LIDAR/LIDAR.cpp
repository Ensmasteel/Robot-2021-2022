#include "LIDAR.h"
#include "Arduino.h"
#include "RPLidar.h"

//au lieu de stocket tous les objets (qui eux meme stocks les points)
//juste stocker leur cg
#define pi 3.1415


point operator+(point pt1,point pt2)
{
    point pt;
    pt.x = pt1.x+pt2.x;
    pt.y = pt1.y+pt2.y;
    return pt;
}
point operator-(point pt1,point pt2)
{
    point pt;
    pt.x = pt1.x-pt2.x;
    pt.y = pt1.y-pt2.y;
    return pt;
}

float dist(point p1,point p2)
{
    float x = p1.x-p2.x;
    float y = p1.y-p2.y;
    return sqrt(x*x+y*y);
};

float dist(point p1)
{
    float x = p1.x;
    float y = p1.y;
    return sqrt(x*x+y*y);
};

float angle(point p1)
{
    float x = p1.x;float y = p1.y;
    //-pi ; pi
    if(x>0){
        return atan(y/x);
    }
    else if(x<0 && y>=0){
        return atan(y/x)+pi;
    }
    else if(x<0 && y<0){
        return atan(y/x)-pi;
    }
    else if(x==0 && y>0){
        return pi/2;
    }
    else if(x==0 && y<0){
        return -pi/2;
    }
    else{
        return 0;
    }
};

float dist(obj o1,obj o2)
{
    return dist(o1.CG,o2.CG);
};

float dist(obj o1)
{
    return dist(o1.CG);
};

float angle(obj o1){
    return angle(o1.CG);
};
float angle_deg(obj o1){
    return angle(o1.CG)/pi*180;
};

point object_data::calculateCG()
{
    float x = 0.0,y = 0.0;
    for(int i=0;i < nbpoint;i++)
    {
        x+= points[i].x;
        y+= points[i].y;
    }
    CG.x=x/nbpoint;
    CG.y=y/nbpoint;
    return CG;
};

bool object_data::addpoint(point pt,float dist_max)
{
    if (nbpoint>nb_points_max){
        return false;//object full
    }
    else if(nbpoint == 0){
        points[nbpoint] = pt;
        nbpoint ++;
        return true; 
    }
    else if(dist(pt,points[nbpoint-1])>dist_max){
        return false; //point too far
    }
    else{
        points[nbpoint] = pt;
        nbpoint ++;
        return true; //add the point
    }
}

void object_data::show(HardwareSerial &serialobj)
{
    uint16_t nbpoint = 0;//number of points
    for (int i =0; i<nbpoint; i++){
        serialobj.print("pt : ");serialobj.print(i);
        serialobj.print("  x: ");serialobj.print(points[i].x);serialobj.print("  y: ");serialobj.println(points[i].y);
    }
}


void LIDAR::init_com(HardwareSerial &serialobj){
    serial_com =  &serialobj;
    serial_com -> begin(baudRate_com);
    delay(100);
    serial_com -> println("comunication started");
    com = true;
}


bool LIDAR::init(HardwareSerial &serialobj){
    int i = 0;
    while (!IS_OK(this -> waitPoint())){
        if (i>10){
            if(com){serial_com -> println("fail");}
            return false;
        }
        analogWrite(this -> motor_pin, 0); //stop the rplidar motor
        // try to detect RPLIDAR... 
        rplidar_response_device_info_t info;
        if (IS_OK(this -> getDeviceInfo(info, 100))) {
            // detected...
            this -> startScan();
            // start motor rotating at max allowed speed
            analogWrite(this -> motor_pin, 255);
            delay(1000);
        }
        else if(com){serial_com -> println("not ok");}
        i++;
        if (com){serial_com -> println(i);}
    }
    if(com){
        serial_com -> print("lidar started after : ");
        serial_com -> print(i);
        serial_com -> println(" tries");
        delay(1000);
    }
    return true;
}


void LIDAR::show_data(){
    if(com && IS_OK(this -> waitPoint())){
        float distance = this ->getCurrentPoint().distance; //distance value in mm unit
        float angle    = this ->getCurrentPoint().angle; //anglue value in degree
        bool  startBit = this ->getCurrentPoint().startBit; //whether this point is belong to a new scan
        byte  quality  = this ->getCurrentPoint().quality; //quality of the current measurement
        serial_com -> print("distance  :");
        serial_com -> println(distance);
    }
}


void LIDAR::scan(){
    if (IS_OK(this -> waitPoint())){
        float distance = getCurrentPoint().distance; //distance value in mm unit
        float angle    = getCurrentPoint().angle; //anglue value in degree
        point pt;
        pt.x = distance*cos(angle/180.0*3.1415);
        pt.y = distance*sin(angle/180.0*3.1415);
        //serial_com -> print("d ");serial_com -> print(distance);serial_com -> print(" O ");serial_com -> println(angle);
        //serial_com -> print("x ");serial_com -> print(pt.x);serial_com -> print(" y ");serial_com -> println(pt.y);

        bool completed = !current_obj.addpoint(pt);

        if (completed){//if object is completed
            //add the object
            list_object[index].CG = current_obj.calculateCG();
            list_object[index].state = true;
            list_object[index].nbpoint = current_obj.nbpoint;
            list_object[index].time = millis();

            index++;
            if(index>= nb_objects_max){index = 0;}

            list_object[index].state = false;
            current_obj.nbpoint = 0;
            current_obj.addpoint(pt);
        }
        else{
            list_object[index].state = false;
        }

    }
}


void LIDAR::show_objects(){
    serial_com -> println("objects : ");
    for (int i = 0; i< nb_objects_max; i++){
        serial_com -> print("objet n :");serial_com -> println(i);
        if(list_object[i].state && com ){    
            serial_com -> print("position  : x : "); serial_com -> print(list_object[i].CG.x);serial_com -> print(" y : ");serial_com -> println(list_object[i].CG.y);  
            serial_com -> print("nbpoints  : "); serial_com -> println(list_object[i].nbpoint);
        }
        else{
        }
    }
}


void LIDAR::filter_objects(int dmin, int dmax, int nb_points_min){

    //retire les objets trop proches, trop loins, trop petits

    point O;
    index_filtered = 0;
    uint32_t t= millis();
    for (int i=0; i<nb_objects_max_filtered;i++){ //deplace les objets et reset la liste
        list_object_filtered_prev[i] = list_object_filtered[i];
        list_object_filtered[i] = empty_object;
    }
    for (int i=0; i<nb_objects_max;i++){
        if (dmin<dist(O,list_object[i].CG) && dist(O,list_object[i].CG)<dmax && list_object[i].nbpoint>nb_points_min && t-list_object[i].time<300)
        {
            list_object_filtered[index_filtered] = list_object[i];
            index_filtered++;
            if(index_filtered >= nb_objects_max_filtered){
                index_filtered = 0;
            }
        }
    }
}

void LIDAR::calcul_speed(){
    for (int i=0; i<nb_objects_max_filtered;i++){
        for (int j=0; j<nb_objects_max_filtered;j++){
            float d = dist(list_object_filtered[i],list_object_filtered_prev[j]);
            if (d<100){
                list_object_filtered[i].speed = list_object_filtered[i].CG-list_object_filtered_prev[j].CG;
            }

        }
    }
}


void LIDAR::show_objects_filtered(){
    serial_com -> println("objects filtered : ");
    for (int i = 0; i< nb_objects_max_filtered; i++){
        serial_com -> print("objet n : ");serial_com -> print(i);
        if(list_object_filtered[i].state && com ){    
            serial_com -> print("   position  : x : "); serial_com -> print(list_object_filtered[i].CG.x);serial_com -> print(" y : ");serial_com -> print(list_object_filtered[i].CG.y);  
            serial_com -> print("   nbpoints  : "); serial_com -> println(list_object_filtered[i].nbpoint);
            serial_com -> print("               vitesse  : x : "); serial_com -> print(list_object_filtered[i].speed.x);serial_com -> print(" y : ");serial_com -> println(list_object_filtered[i].speed.y);  
            serial_com -> println("");
        }
        else{
            serial_com -> println("");
        }
    }
    serial_com -> println("");
}

void LIDAR::plot(){
    for (int i = 0; i< 3; i++)
    {
        serial_com -> print(list_object_filtered[0].CG.x);
        serial_com -> print("\t");
        serial_com -> print(list_object_filtered[0].CG.y);
        serial_com -> print("\t");
        serial_com -> print(list_object_filtered[0].nbpoint);
        serial_com -> print("\t");
    }
    serial_com -> println("");

}

void LIDAR::plot_biggest(){
    obj ob = list_object_filtered[0];
    for (int i = 0; i< nb_objects_max_filtered ; i++)
    {
        if(ob.nbpoint<list_object_filtered[i].nbpoint){
            ob=list_object_filtered[i];
        }
        
    }
    serial_com -> print("bigest :");
    serial_com -> print(dist(ob));
    serial_com -> print("\t");
    serial_com -> print(angle_deg(ob));
    serial_com -> print("\t");
    serial_com -> print(ob.nbpoint);
    serial_com -> print("\t");
    serial_com -> println("");
}

void LIDAR::plot_closest(){
    obj ob = list_object_filtered[0];
    for (int i = 0; i< nb_objects_max_filtered ; i++)
    {
        if(dist(ob)>dist(list_object_filtered[i]) && dist(list_object_filtered[i])>1){
            ob=list_object_filtered[i];
        }
        
    }
    serial_com -> print("closest :");
    serial_com -> print(dist(ob));
    serial_com -> print("\t");
    serial_com -> print(angle_deg(ob));
    serial_com -> print("\t");
    serial_com -> print(ob.nbpoint);
    serial_com -> print("\t");
    serial_com -> println("");
}


void LIDAR::order_by(char16_t methode){

    if (methode == 'd')
    {
        point O;
        int distances[nb_objects_max_filtered];
        for (int i; i<nb_objects_max_filtered; i ++){
            distances[i] = dist(list_object_filtered[i].CG,O);}
        for (int i=0;i<nb_objects_max_filtered; i++){
            list_object_ordered[i] = list_object_filtered[i];}
        
        int a;
        obj b;

        boolean swaped = true;
        while (swaped){
            swaped = false;
            for (int i=0; i<nb_objects_max_filtered -1; i ++){
                if (distance[i]>distance[i+1]){
                    swaped = true;
                    a = distance[i]; distance[i] = distance[i+1]; distance[i+1] = a;//swap distances
                    b = list_object_filtered[i]; list_object_filtered[i] = list_object_filtered[i+1]; list_object_filtered[i+1] = b;//swap objects                 
                }
            }
        }
    }
}