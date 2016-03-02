
left = 0;
right = 0;

//We only have goal position
//Here we calculate the angle from the positions
dy = yg-y0;
dx = xg-x0;

goaltheta = atan(dy/dx)*180/M_PI;
if (dx==0){
	if (dy<0){
	goaltheta=-90;	
	}
	if (dy>0){
	goaltheta=90;	
	}
}

if(dx <  0){
	goaltheta+=180;
}

if(goaltheta <= -180) goaltheta += 360; 
if(goaltheta > 180) goaltheta -= 360;

dy = yg-y;
dx = xg-x;

//All this mess is due to atan
if(dx < 0){ 
  desiredtheta = atan(dy/dx)*180/M_PI;
  if(dy < 0){
    desiredtheta +=180;
  }
  else{
    desiredtheta -=180;
  } 
 }else{
  if(dx != 0){
    desiredtheta = atan(dy/dx)*180/M_PI;
  }else{
    desiredtheta = desiredtheta;
  }
 }
//if(dx <  0){
	//desiredtheta+=180;
//}
if (dx==0){
	if (dy<0){
	desiredtheta=-90;	
	}
	if (dy>0){
	desiredtheta=90;	
	}
}
//Our signal is limited to -180 to 180
if(theta < -170){
	if(desiredtheta > 180) desiredtheta = desiredtheta*-1;
}
else  if(theta > 170){
	if(desiredtheta <= -180) desiredtheta = desiredtheta*-1; 	
}
else{
 if(desiredtheta <= -180) desiredtheta += 360; 
 if(desiredtheta > 180) desiredtheta -= 360;
}
//Controller for rotation
K_rot = 0.5f*L/R;
//K = 3.0f;
u_rot = K_rot*(desiredtheta -theta);


//Controller for translation
K_trans = 3.0f;
K_p = 7.0f;
p = 50.0f;
d0 = (cos(theta*M_PI/180) *(x0 -x) + sin(theta*M_PI/180)* (y0 -y));
dg = (cos(goaltheta*M_PI/180) *(xg -x) + sin(goaltheta*M_PI/180)* (yg -y));
dp = sin(goaltheta*M_PI/180)*(x+p*cos(theta*M_PI/180) -x0) - cos(goaltheta*M_PI/180)*(y+p*sin(theta*M_PI/180) -y0);
//u_rot = dp*K_p;
//u_trans = dg*K_trans;
//u_trans = K_trans*d0;
//////////////////////////////////////////  HYBRID CONTROLLER /////////////////////


switch(state){
	case 0:
		//Rotate
		//if(dy == 0 && dx < 0) desiredtheta = -180;
		u_rot = K_rot *	(desiredtheta - theta);
		u_trans = K_trans * d0;
		
		if(fabs((desiredtheta -theta)) < 5 && fabs(x-x0)<2 && fabs(y-y0)<2){
			state = 1;		
		}
		Serial.print("IN CASE 0");
		break;

	case 1:
		//Translate
		u_rot = K_p * dp;
		u_trans = K_trans * dg;

		if(fabs((xg -x)) < 0.5 && fabs((yg -y)) < 0.5){
			state = 2;
			old_xg = xg;
			old_yg = yg;		
		}
		Serial.print("IN CASE 1\n");
		break;
	
	case 2:
		//Wait
		u_rot = 0;
		u_trans = 0;
		Serial.print("IN CASE 2");
		break;
}


/////////////////////////////////////////////////////////////////////////////////////

left += round(u_trans);
right += round(u_trans);

left += round(-u_rot);
right +=round(u_rot);

//Debugprints to MainWindow.
Serial.print("\nOWN:");
Serial.print(desiredtheta-theta, DEC);
Serial.print(";");
Serial.print(d0, DEC);
Serial.print(";\n");
Serial.print("goaltheta=(degree)");
Serial.print(desiredtheta, DEC);
Serial.print(";\n");
Serial.print("sin theta");
Serial.print(sin(theta), DEC);
Serial.print(";\n");
Serial.print("cos theta");
Serial.print(cos(theta), DEC);
Serial.print(";\n");
Serial.print("xg = ");
Serial.print(xg);
Serial.print("\n x = ");
Serial.print(x);
