
left = 0;
right = 0;

//We only have goal position
//Here we calculate the angle from the positions
dy = yg-y;
dx = xg-x;

//All this mess is due to atan
if(dx < 0){ 
  goaltheta = atan(dy/dx)*180/M_PI;
  if(dy < 0){
    goaltheta +=180;
  }
  else{
    goaltheta -=180;
  } 
 }else{
  if(dx != 0){
    goaltheta = atan(dy/dx)*180/M_PI;
  }else{
    goaltheta = goaltheta;
  }
 }

//Our signal is limited to -180 to 180
if(goaltheta < 180) goaltheta += 360; 
if(goaltheta > 180) goaltheta -= 360;


//Controller for rotation
K_rot = 0.7f*L/R;
//K = 3.0f;
//u_rot = K_rot*(goaltheta -theta);

//Controller for translation
K_trans = 5.0f;
K_p = 5.0f;
p = 5.0f;
d0 = (cos(theta*M_PI/180) *(xg -x) + sin(theta*M_PI/180)* (yg -y));
dp = sin(goaltheta*M_PI/180)*(x+p*cos(theta*M_PI/180) -x0) -
  cos(goaltheta*M_PI*180)*(y+p*sin(theta*M_PI/180) -y0);
u_rot = dp*K_p;
u_trans = d0*K_trans;
//u_trans = K_trans*d0;
left += round(u_trans);
right += round(u_trans);

left += round(-u_rot);
right +=round(u_rot);

//Debugprints to MainWindow.
Serial.print("OWN:");
Serial.print(goaltheta-theta, DEC);
Serial.print(";");
Serial.print(d0, DEC);
Serial.print(";\n");
Serial.print("goaltheta=(degree)");
Serial.print(goaltheta, DEC);
Serial.print(";\n");
Serial.print("sin theta");
Serial.print(sin(theta), DEC);
Serial.print(";\n");
Serial.print("cos theta");
Serial.print(cos(theta), DEC);
Serial.print(";\n");
