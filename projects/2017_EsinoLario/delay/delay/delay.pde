/* ** Delay ** by Alberto Morabito
 * Project made at the wikicourse "Design and program interactive objects", held by Marco Lurati and Giovanni Profeta in Esino Lario (Italy), on July 2017.

  This project is based only in Processing

 */


// Read data from the serial port and change the color of a rectangle 
// when a switch connected to the board is pressed and released
 
import processing.serial.*; 
 
Serial port;                             // Create object from Serial class 
float smax=0.0; 
int   low=16;
float g0=4.9;
float tmax=0.0;
float acc=0.0;
float heigh=640;
float width=680;
float t=0;
float s=0.0;
float tilt=0;
float L=0.0;
float dy=0.0;
float dx=0.0;
float P=0.0;
int   val=0;
float H=heigh;

void setup() { 
     size(680, 640); 
     frameRate(25);
  /*   String portNAME = Serial.list()[3];
     println(Serial.list()[3]);
 
  // Open the port that the board is connected to and use the same speed (115200 bps) 
      port = new Serial(this, portNAME, 115200); 
 */   
        background(255);
} 


void draw() {
  
  /*
  if (0 < port.available()) {         // If data is available,
    val = port.read();    // read it and store it in val
    println(val); 
   }
   */
   if (val==0){
                val=180;
                tmax=t;
                t=0.0;
                fill(64, 16, 0);
                rect(30, 450, 20, 20);  
     
   }
    else if(H<=0) { H=width;}
  
    L=sqrt(width*width+(H-low)*(H-low));
    P=L/(H-low);
    tilt=1.0/P;
   line(0,low,L,H);
  background(255);  // Set background to white  
  fill(204);
  noStroke();
  rect(5, heigh-65, 480, 45);
  fill(0);
  stroke(0);
  
  s=sqrt(dy*dy+dx*dx);
  textSize(24);
   text("Final t ="+nf(tmax, 0, 2)+" Final space = "+nf(smax, 0, 2)+" accel = "+nf(acc, 0, 2), 10, heigh-220);
   text("Tilt="+nf(tilt, 0, 2), 10, heigh-90);
  //text(tilt, 80, heigh-90);
   text("time="+nf(t, 0, 2), 10, heigh-180);
  
  strokeWeight(10);
  
  t=t+0.1;
  s=t*t*g0*tilt;
  dy=s*tilt;
  dx=dy*P;
  line(0,low,L,H);
  ellipse(dx,dy-35+low, 50, 50);
  fill(204, 102, 0);
     rect(30, 480, 20, 20);           
   if (dx<width){
       text("s ="+nf(dx, 0, 2)+"    "+nf(L,0,2), 10, heigh-30);
     }
 else {
                 H=H-100;
                 acc=2.0*s/t/t;
                  tmax=t;
text("Final t ="+nf(tmax, 0, 2)+" Final space = "+nf(smax, 0, 2)+" accel = "+nf(acc, 0, 2), 10, heigh-220);
                  t=0.0;
                  smax=s;
                  delay(2000);
//             if(val==0) { H=heigh; }
           
  }
  delay(10);
}