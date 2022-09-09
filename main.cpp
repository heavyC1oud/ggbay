#include <iostream>
#include <unistd.h>

using namespace std;

int main()
{
    cout<<"IT's GGBay\n";


    int ret;

    ret = execl("/home/vitalii/.local/bin/streamlink", "streamlink", "--player", "cvlc --fullscreen", "goodgame.ru/channel/Neldoreth_inf", "best", NULL);

    cout<< ret;

    return 0;
}