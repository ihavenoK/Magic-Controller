# Language and Translation Rules

- **Target Language**: Simplified Chinese (简体中文)
- **Directives**:
  1. **User Interaction**: Always respond to the user in clear, professional Simplified Chinese.
  2. **Reasoning & Planning**: Write all plans, reasoning steps, code commentaries, and explanations in Simplified Chinese.
  3. **CLI Prompts & Tasks**: All planning files (`task.md`), task summaries, and instructions sent by the CLI must be generated and processed in Simplified Chinese.
  4. **Code Comments**: Keep code comments in English unless translating them to Chinese is explicitly requested by the user, to ensure compilation compatibility.
  5. **Permission & Tool Requests**: When invoking tools that require user confirmation or show messages in the terminal (like `ask_permission` or `run_command`), the `Reason` (申请原因), `toolAction` (工具操作摘要), and `toolSummary` (工具简述) parameters MUST be written in Simplified Chinese so the CLI prompts display in Chinese.

