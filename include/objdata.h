#ifndef OBJDATA_H
#define OBJDATA_H


class objdata
{
    public:
        objdata();
        virtual ~objdata();

        int y;
        int x;
        int tile;
        int palette;
        int xflip;
        int yflip;
        int prio;
        int num;

    protected:

    private:
};

#endif // OBJDATA_H
