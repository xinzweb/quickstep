/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_PATTERN_MATCHER_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_PATTERN_MATCHER_HPP_

#include <memory>
#include <type_traits>

#include "query_optimizer/physical/PhysicalType.hpp"

namespace quickstep {
namespace optimizer {
namespace physical {

class Aggregate;
class BinaryJoin;
class CopyFrom;
class CreateTable;
class DeleteTuples;
class DropTable;
class HashJoin;
class InsertTuple;
class Join;
class NestedLoopsJoin;
class Selection;
class SharedSubplanReference;
class Sort;
class TableReference;
class TopLevelPlan;
class UpdateTable;

/** \addtogroup OptimizerPhysical
 *  @{
 */

/**
 * @brief Templated matcher for each Physical node class.
 *
 * @param PhysicalClass The Physical class for the physical node to be matched with.
 * @param physical_types All the physical types of the Physical class.
 */
template <class PhysicalClass, PhysicalType... physical_types>
class SomePhysicalNode {
 public:
  /**
   * @brief Checks whether the object managed in \p physical is an instance
   *        of the template argument PhysicalClass by checking whether
   *        it is one of types in the given template arguments physical_types.
   *
   * @param physical The physical node to be checked.
   * @return True for a match; otherwise false.
   */
  template <class OtherPhysicalClass>
  static bool Matches(const std::shared_ptr<const OtherPhysicalClass> &physical) {
    for (const PhysicalType physical_type : kPhysicalTypes) {
      if (physical->getPhysicalType() == physical_type) {
        return true;
      }
    }
    return false;
  }

  /**
   * @brief Checks whether the object managed in \p physical is an instance
   *        of the template argument PhysicalClass by checking whether
   *        it is one of types in the given template arguments physical_types,
   *        If true, it additionally casts \p physical to a std::shared_ptr
   *        \p cast_physical of the template argument PhysicalClass.
   *
   * @param physical The physical node to be checked.
   * @param cast_physical The cast physical node.
   * @return True if the object managed in \p physical is an instance of PhysicalClass.
   */
  template <class OtherPhysicalClass>
  static bool MatchesWithConditionalCast(const std::shared_ptr<const OtherPhysicalClass> &physical,
                                         std::shared_ptr<const PhysicalClass> *cast_physical) {
    bool is_match = Matches(physical);
    if (is_match) {
      *cast_physical = std::static_pointer_cast<const PhysicalClass>(physical);
    }
    return is_match;
  }

 private:
  constexpr static PhysicalType kPhysicalTypes[] = {physical_types...};
};

template <class PhysicalClass, PhysicalType... physical_types>
constexpr PhysicalType SomePhysicalNode<PhysicalClass, physical_types...>::kPhysicalTypes[];

// Specializations for all Physical classes.

using SomeAggregate = SomePhysicalNode<Aggregate, PhysicalType::kAggregate>;
using SomeBinaryJoin = SomePhysicalNode<BinaryJoin, PhysicalType::kHashJoin, PhysicalType::kNestedLoopsJoin>;
using SomeCopyFrom = SomePhysicalNode<CopyFrom, PhysicalType::kCopyFrom>;
using SomeCreateTable = SomePhysicalNode<CreateTable, PhysicalType::kCreateTable>;
using SomeDeleteTuples = SomePhysicalNode<DeleteTuples, PhysicalType::kDeleteTuples>;
using SomeDropTable = SomePhysicalNode<DropTable, PhysicalType::kDropTable>;
using SomeHashJoin = SomePhysicalNode<HashJoin, PhysicalType::kHashJoin>;
using SomeInsertTuple = SomePhysicalNode<InsertTuple, PhysicalType::kInsertTuple>;
using SomeJoin = SomePhysicalNode<Join, PhysicalType::kHashJoin, PhysicalType::kNestedLoopsJoin>;
using SomeNestedLoopsJoin = SomePhysicalNode<NestedLoopsJoin, PhysicalType::kNestedLoopsJoin>;
using SomeSelection = SomePhysicalNode<Selection, PhysicalType::kSelection>;
using SomeSharedSubplanReference = SomePhysicalNode<SharedSubplanReference, PhysicalType::kSharedSubplanReference>;
using SomeSort = SomePhysicalNode<Sort, PhysicalType::kSort>;
using SomeTableReference = SomePhysicalNode<TableReference, PhysicalType::kTableReference>;
using SomeTopLevelPlan = SomePhysicalNode<TopLevelPlan, PhysicalType::kTopLevelPlan>;
using SomeUpdateTable = SomePhysicalNode<UpdateTable, PhysicalType::kUpdateTable>;

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_PATTERN_MATCHER_HPP_ */