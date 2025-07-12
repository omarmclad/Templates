int a,b,c,d;
int func(int x,int y,int xx,int yy){
  //area
    return(abs(y-yy)*abs(x-xx));
}
void unionRectangle(int x,int y,int xx,int yy,int x2,int y2,int xx2,int yy2){
    a = max(x,x2);
    b = max(y,y2);
    c = min(xx, xx2);
    d = min(yy,yy2);
    // make sure that
    // if (a<c&&b<d) {
    //   if yes then subtract the area
    // }
}
