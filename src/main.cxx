
#include "controller.hxx"
#include <ge211.hxx>

int
main()
{
    Model model;
    Controller controller(model);
    controller.run();

    return 0;
}
