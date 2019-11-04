#include <torch/optim/adagrad.h>

#include <torch/csrc/autograd/variable.h>
#include <torch/serialize/archive.h>
#include <torch/utils.h>

#include <ATen/ATen.h>

#include <functional>

namespace torch {
namespace optim {

AdagradOptions::AdagradOptions(double learning_rate)
    : learning_rate_(learning_rate) {}

/// Adapted from
/// https://github.com/pytorch/pytorch/blob/master/torch/optim/adagrad.py
void Adagrad::step() {
  // for (auto& group : param_groups) {
  //   AdagradOptions options = convert_ivalue_to_options(group.at("options"));
  //   for (auto p : group.at("params").toTensorList()) {
  //     if (!p.grad().defined()) {
  //       continue;
  //     }
  //     auto grad = p.grad().data();
  //     auto& state_ = state.at(p.unsafeGetTensorImpl());
  //     state_.at("step").toInt()+=1;
  //
  //     if(options.weight_decay() != 0) {
  //       TORCH_CHECK(!p.grad().data().is_sparse(), "weight_decay option is not compatible with sparse gradients");
  //       NoGradGuard guard;
  //       grad += options.weight_decay() * p.data();
  //     }
  //
  //     const auto clr =  options.learning_rate() /
  //         (1.0 + (state_.at("step").toInt() - 1.0) * group.at("options").lr_decay());
  //
  //     auto& sum = state_.at("sum").toTensor();
  //
  //     if(grad.is_sparse()) {
  //       grad = grad.coalesce();
  //       auto grad_indices = grad._indices();
  //       auto grad_values = grad._values();
  //       auto size = grad.size();
  //
  //       [&] make_sparse(Tensor values) -> Tensor /*confirm*/ {
  //         auto constructor = grad.new(); //confirm
  //         if(grad_indices.dim() == 0 || values.dim() == 0) {
  //           return constructor().resize_as_(grad);
  //         }
  //         return constructor(grad_indices, values, size);
  //       }
  //       sum.add_(make_sparse(grad_values.pow(2)));
  //       auto std = state_.at("sum").toTensor().sparse_mask(grad);
  //       const auto std_values = std.sqrt().add_(options.eps());
  //       p.data().add_(-clr, make_sparse(grad_values / std_values));
  //     }
  //     else {
  //       sum.addcmul_(grad, grad, 1.0);
  //       const auto std = state_.at("sum").toTensor().sqrt().add_(options.eps());
  //       p.data().addcdiv_(-clr, grad, std);
  //     }
  //   }
  //   group.at("options") = convert_options_to_dict(options);
  // }
}

void Adagrad::save(serialize::OutputArchive& archive) const {
  serialize(*this, archive);
}

void Adagrad::load(serialize::InputArchive& archive) {
  serialize(*this, archive);
}

// typedef double (*fnPtr)(double);
//
// AdagradOptions convert_ivalue_to_options(at::IValue ivalue) {
//     c10::Dict<at::IValue, at::IValue> dict = ivalue.toGenericDict();
//     c10::Dict<std::string, fnPtr(double)> optionsMap;
//     optionsMap.insert("learning_rate", learning_rate);
//     optionsMap.insert("lr_decay", lr_decay);
//     optionsMap.insert("weight_decay", weight_decay);
//     optionsMap.insert("initial_accumulator_value", initial_accumulator_value);
//     optionsMap.insert("eps", eps);
//
//     AdagradOptions options;
//     for(auto key : dict) {
//       auto fnPtr_ = optionsMap.at(key.toString())
//       options.fnPtr_(dict.at(key).toDouble());
//     }
//     return options;
// }

void convert_options_to_ivalue(AdagradOptions options) {
    c10::Dict<std::string, at::IValue> dict;
    dict.insert("learning_rate", options.learning_rate());
    dict.insert("lr_decay", options.lr_decay());
    dict.insert("weight_decay", options.weight_decay());
    dict.insert("initial_accumulator_value", options.initial_accumulator_value());
    dict.insert("eps", options.eps());
    //return dict;
}
} // namespace optim
} // namespace torch
