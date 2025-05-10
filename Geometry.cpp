//Point Locator
//Line passing through (x1,y1),(x2,y2) 
//for point (x3,y3) check whether it is on the line or above it (left) or below it (right)
void pointLocationTest(int x1,int y1,int x2,int y2,int x3,int y3){
    int ls=(y3-y1)*(x2-x1);
    int rs=(x3-x1)*(y2-y1);
    ls-=rs;
    if(ls==0){
        cout<<"TOUCH"<<endl;
        return;
    }
    if(ls>0){
        cout<<"LEFT"<<endl;
        return;
    }
    if(ls<0){
        cout<<"RIGHT"<<endl;
        return;
    }
}
