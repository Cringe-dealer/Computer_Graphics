#include "reform_manager.hpp"
#include "transform_matrix.hpp"
#include <math.h>

std::shared_ptr<reform_manager> reform_manager_creator::get_manager()
{
    if (_manager == nullptr)
        create_manager();

    return _manager;
}

void reform_manager_creator::create_manager()
{
    static std::shared_ptr<reform_manager> _manager(new reform_manager());
    this->_manager = _manager;
}

void reform_manager::shift_model(std::shared_ptr<object> obj, point &move)
{
    std::shared_ptr<Matrix<double>> reform_mtr(std::make_shared<MoveMatrix>(move));
    obj->reform(reform_mtr);
}

void reform_manager::scale_model(std::shared_ptr<object> obj, point &scale)
{
    std::shared_ptr<Matrix<double>> reform_mtr(std::make_shared<ScaleMatrix>(scale));
    obj->reform(reform_mtr);
}

void reform_manager::rotate_model(std::shared_ptr<object> obj, point &rotate)
{
    point angles = rotate.deg_to_rad();
    std::shared_ptr<Matrix<double>> reform_mtr(std::make_shared<RotateOxMatrix>(angles.get_x()));
    obj->reform(reform_mtr);

    reform_mtr = std::make_shared<RotateOyMatrix>(angles.get_y());
    obj->reform(reform_mtr);

    reform_mtr = std::make_shared<RotateOzMatrix>(angles.get_z());
    obj->reform(reform_mtr);
}
