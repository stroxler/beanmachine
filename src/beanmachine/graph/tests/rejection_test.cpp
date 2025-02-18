/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <array>

#include <gtest/gtest.h>

#include "beanmachine/graph/graph.h"

using namespace beanmachine::graph;

TEST(testrejection, beta_bernoulli) {
  Graph g;
  uint a = g.add_constant_pos_real(2.0);
  uint b = g.add_constant_pos_real(3.0);
  uint prior = g.add_distribution(
      DistributionType::BETA,
      AtomicType::PROBABILITY,
      std::vector<uint>({a, b}));
  uint prob = g.add_operator(OperatorType::SAMPLE, std::vector<uint>({prior}));
  uint n = g.add_constant_natural(5);
  uint like = g.add_distribution(
      DistributionType::BINOMIAL,
      AtomicType::NATURAL,
      std::vector<uint>({n, prob}));
  uint k = g.add_operator(OperatorType::SAMPLE, std::vector<uint>({like}));
  g.observe(k, (natural_t)2);
  g.query(prob);
  auto& means = g.infer_mean(1000, InferenceType::REJECTION, 23891);
  // TODO: Insert closed form formula here. -- Mootaz Elnozahy
  EXPECT_NEAR(means[0], 0.4, 1e-2);
}
