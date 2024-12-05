//
// Created by frio on 04/12/2024.
//

#ifndef I_UICOMPONENT_H
#define I_UICOMPONENT_H
namespace Cold {
    class i_UIComponent {
    public:
        virtual void render() = 0;
        virtual ~i_UIComponent() {}
    };
}

#endif //I_UICOMPONENT_H
