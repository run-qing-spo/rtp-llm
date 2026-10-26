#include "rtp_llm/cpp/cache/block_tree_cache/transfer/TransferTaskRegistry.h"
#include <gtest/gtest.h>

namespace rtp_llm {
TEST(TransferTaskRegistryTest, TracksCompletionWithoutRestartingTask) {
    TransferTaskRegistry registry;
    using State = TransferTaskRegistry::State;
    EXPECT_EQ(registry.query("task"), State::NOT_FOUND);
    EXPECT_FALSE(registry.finish("task"));
    EXPECT_EQ(registry.query("task"), State::NOT_FOUND);
    EXPECT_FALSE(registry.start(""));
    ASSERT_TRUE(registry.start("task"));
    EXPECT_FALSE(registry.start("task"));
    EXPECT_EQ(registry.query("task"), State::RUNNING);
    ASSERT_TRUE(registry.start("other"));
    ASSERT_TRUE(registry.finish("task"));
    EXPECT_TRUE(registry.finish("task"));
    EXPECT_FALSE(registry.start("task"));
    EXPECT_EQ(registry.query("task"), State::FINISHED);
    EXPECT_EQ(registry.query("other"), State::RUNNING);
}
}  // namespace rtp_llm
