/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_HASHJOIN_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_HASHJOIN_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/physical/BinaryJoin.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace physical {

/** \addtogroup OptimizerPhysical
 *  @{
 */

class HashJoin;
typedef std::shared_ptr<const HashJoin> HashJoinPtr;

/**
 * @brief Physical hash join node.
 */
class HashJoin : public BinaryJoin {
 public:
  PhysicalType getPhysicalType() const override { return PhysicalType::kHashJoin; }

  std::string getName() const override { return "HashJoin"; }

  /**
   * @brief Join attributes in the left logical 'left_'.
   */
  const std::vector<expressions::AttributeReferencePtr>& left_join_attributes() const {
    return left_join_attributes_;
  }

  /**
   * @brief Join attributes in the right logical 'right_'.
   */
  const std::vector<expressions::AttributeReferencePtr>& right_join_attributes() const {
    return right_join_attributes_;
  }

  /**
   * @brief The filtering predicate evaluated after join.
   */
  const expressions::PredicatePtr& residual_predicate() const {
    return residual_predicate_;
  }

  PhysicalPtr copyWithNewChildren(
      const std::vector<PhysicalPtr> &new_children) const override {
    DCHECK_EQ(children().size(), new_children.size());
    return Create(new_children[0],
                  new_children[1],
                  left_join_attributes_,
                  right_join_attributes_,
                  residual_predicate_,
                  project_expressions());
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override;

  bool maybeCopyWithPrunedExpressions(
      const expressions::UnorderedNamedExpressionSet &referenced_expressions,
      PhysicalPtr *output) const override;

  /**
   * @brief Creates a physical HashJoin. The left/right operand does not correspond to
   *        probe/build operand.
   *
   * @param left The left operand.
   * @param right The right operand.
   * @param left_join_attributes The join attributes in the 'left'.
   * @param right_join_attributes The join attributes in the 'right'.
   * @param residual_predicate Optional filtering predicate evaluated after join.
   * @param project_expressions The project expressions.
   * @return An immutable physical HashJoin.
   */
  static HashJoinPtr Create(
      const PhysicalPtr &left,
      const PhysicalPtr &right,
      const std::vector<expressions::AttributeReferencePtr> &left_join_attributes,
      const std::vector<expressions::AttributeReferencePtr> &right_join_attributes,
      const expressions::PredicatePtr &residual_predicate,
      const std::vector<expressions::NamedExpressionPtr> &project_expressions) {
    return HashJoinPtr(
        new HashJoin(left,
                     right,
                     left_join_attributes,
                     right_join_attributes,
                     residual_predicate,
                     project_expressions));
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const override;

 private:
  HashJoin(
      const PhysicalPtr &left,
      const PhysicalPtr &right,
      const std::vector<expressions::AttributeReferencePtr> &left_join_attributes,
      const std::vector<expressions::AttributeReferencePtr> &right_join_attributes,
      const expressions::PredicatePtr &residual_predicate,
      const std::vector<expressions::NamedExpressionPtr> &project_expressions)
      : BinaryJoin(left, right, project_expressions),
        left_join_attributes_(left_join_attributes),
        right_join_attributes_(right_join_attributes),
        residual_predicate_(residual_predicate) {}

  std::vector<expressions::AttributeReferencePtr> left_join_attributes_;
  std::vector<expressions::AttributeReferencePtr> right_join_attributes_;
  expressions::PredicatePtr residual_predicate_;

  DISALLOW_COPY_AND_ASSIGN(HashJoin);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_HASHJOIN_HPP_ */