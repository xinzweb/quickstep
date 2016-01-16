/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_DROPTABLE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_DROPTABLE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class CatalogRelation;

namespace optimizer {
namespace logical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class DropTable;
typedef std::shared_ptr<const DropTable> DropTablePtr;

/**
 * @brief Drops a table.
 */
class DropTable : public Logical {
 public:
  LogicalType getLogicalType() const override { return LogicalType::kDropTable; }

  std::string getName() const override { return "DropTable"; }

  /**
   * @return Gets the catalog relation to be dropped.
   */
  const CatalogRelation* catalog_relation() const { return catalog_relation_; }

  LogicalPtr copyWithNewChildren(
      const std::vector<LogicalPtr> &new_children) const override {
    DCHECK(new_children.empty());
    return Create(catalog_relation_);
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return std::vector<expressions::AttributeReferencePtr>();
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    return std::vector<expressions::AttributeReferencePtr>();
  }

  /**
   * @brief Creates a DropTable logical node that drops \p catalog_relation.
   *
   * @param catalog_relation The relation to be dropped.
   * @return An immutable DropTable node.
   */
  static DropTablePtr Create(const CatalogRelation *catalog_relation) {
    return DropTablePtr(new DropTable(catalog_relation));
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
  explicit DropTable(const CatalogRelation *catalog_relation)
      : catalog_relation_(catalog_relation) {}

  const CatalogRelation *catalog_relation_;

  DISALLOW_COPY_AND_ASSIGN(DropTable);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_DROPTABLE_HPP_ */