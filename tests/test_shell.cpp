#include "../include/shell.h"
#include <gtest/gtest.h>

TEST(IntelligentShellTest, CommandParsing) {
    IntelligentShell shell;
    
    auto tokens = shell.parse_command("ls -la /home/user");
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0], "ls");
    EXPECT_EQ(tokens[1], "-la");
    EXPECT_EQ(tokens[2], "/home/user");
}

TEST(IntelligentShellTest, PipeDetection) {
    IntelligentShell shell;
    
    EXPECT_TRUE(shell.has_pipes("ls | grep test"));
    EXPECT_FALSE(shell.has_pipes("ls -la"));
}
