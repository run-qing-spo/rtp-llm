#pragma once
#include <algorithm>
#include <memory>
#include <vector>
#include "rtp_llm/cpp/cache/block_tree_cache/block_pool/IBlockPool.h"
#include "rtp_llm/cpp/cache/block_tree_cache/transfer/TransferTypes.h"

namespace rtp_llm {
struct FrozenTask {
    struct Block {
        std::shared_ptr<IBlockPool> pool;
        BlockIdxType block;
    };
    explicit FrozenTask(const TransferResult& result):
        finalFrozen(result.status.code() != ErrorCode::TRANSFER_COMPLETION_UNKNOWN || result.recovery_task_ids.empty()),
        recovery_task_ids(result.recovery_task_ids) {}

    void freeze(std::shared_ptr<IBlockPool> pool, BlockIdxType block) {
        if (std::any_of(blocks.begin(), blocks.end(), [&](const Block& b) {
                return b.pool == pool && b.block == block;
            })) return;
        blocks.push_back({std::move(pool), block});
        blocks.back().pool->freezeBlock(block);
    }

    bool finalFrozen;
    std::vector<std::string> recovery_task_ids;
    std::vector<Block> blocks;
};
}  // namespace rtp_llm
