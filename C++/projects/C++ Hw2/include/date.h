#ifndef DATE_H
#define DATE_H

class Date 
{ 
public:
    int d, m, y; 

    Date() = default;
    Date(int d, int m, int y);
}; 

int get_difference(Date dt1, Date dt2);

#endif // !DATE_H
