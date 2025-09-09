package vm

import (
	"testing"
	"github.com/PaulTKoenig/PgaQL_Backend/compiler"
)

func TestExecute_BasicQuery(t *testing.T) {
	instructions := []compiler.Instruction{
		{Op: compiler.OP_SCAN, Args: []interface{}{"testdata/test_players"}},
		{Op: compiler.OP_LOAD_FIELD, Args: []interface{}{"season"}},
		{Op: compiler.OP_LOAD_CONST, Args: []interface{}{"2025"}},
		{Op: compiler.OP_EQ},
		{Op: compiler.OP_FILTER},
		{Op: compiler.OP_PROJECT, Args: []interface{}{"pts", "date"}},
		{Op: compiler.OP_OUTPUT},
	}

	results, err := Execute(instructions)

	if err != nil {
		t.Fatalf("Expected no error, got: %v", err)
	}

	expectedCount := 2 // Players with season 2025
	if len(results) != expectedCount {
		t.Errorf("Expected %d results, got %d", expectedCount, len(results))
	}

	// Verify structure of results
	if len(results) > 0 {
		result := results[0]
		if _, exists := result["pts"]; !exists {
			t.Error("Expected 'pts' field in result")
		}
		if _, exists := result["date"]; !exists {
			t.Error("Expected 'date' field in result")
		}
		if len(result) != 2 {
			t.Errorf("Expected 2 fields in result, got %d", len(result))
		}
	}
}

func TestExecute_EmptyInstructions(t *testing.T) {
	instructions := []compiler.Instruction{}

	_, err := Execute(instructions)

	if err == nil {
		t.Error("Expected error for empty instructions")
	}
	if err.Error() != "program must begin with SCAN" {
		t.Errorf("Expected 'program must begin with SCAN', got: %s", err.Error())
	}
}

func TestExecute_NoScanFirst(t *testing.T) {
	instructions := []compiler.Instruction{
		{Op: compiler.OP_LOAD_FIELD, Args: []interface{}{"season"}},
	}

	_, err := Execute(instructions)

	if err == nil {
		t.Error("Expected error when not starting with SCAN")
	}
	if err.Error() != "program must begin with SCAN" {
		t.Errorf("Expected 'program must begin with SCAN', got: %s", err.Error())
	}
}

func TestExecute_MissingField(t *testing.T) {
	instructions := []compiler.Instruction{
		{Op: compiler.OP_SCAN, Args: []interface{}{"testdata/test_players"}},
		{Op: compiler.OP_LOAD_FIELD, Args: []interface{}{"nonexistent_field"}},
		{Op: compiler.OP_OUTPUT},
	}

	_, err := Execute(instructions)

	if err == nil {
		t.Error("Expected error for missing field")
	}
	if err.Error() != "Field 'nonexistent_field' not found" {
		t.Errorf("Expected missing field error, got: %s", err.Error())
	}
}

func TestExecute_StackUnderflow_EQ(t *testing.T) {
	instructions := []compiler.Instruction{
		{Op: compiler.OP_SCAN, Args: []interface{}{"testdata/test_players"}},
		{Op: compiler.OP_LOAD_FIELD, Args: []interface{}{"season"}},
		{Op: compiler.OP_EQ}, // Only 1 operand on stack
		{Op: compiler.OP_OUTPUT},
	}

	_, err := Execute(instructions)

	if err == nil {
		t.Error("Expected error for EQ stack underflow")
	}
	if err.Error() != "EQ requires 2 operands" {
		t.Errorf("Expected EQ error, got: %s", err.Error())
	}
}

func TestExecute_StackUnderflow_AND(t *testing.T) {
	instructions := []compiler.Instruction{
		{Op: compiler.OP_SCAN, Args: []interface{}{"testdata/test_players"}},
		{Op: compiler.OP_LOAD_CONST, Args: []interface{}{true}},
		{Op: compiler.OP_AND}, // Only 1 operand on stack
		{Op: compiler.OP_OUTPUT},
	}

	_, err := Execute(instructions)

	if err == nil {
		t.Error("Expected error for AND stack underflow")
	}
	if err.Error() != "AND requires 2 boolean operands" {
		t.Errorf("Expected AND error, got: %s", err.Error())
	}
}

func TestExecute_StackUnderflow_OR(t *testing.T) {
	instructions := []compiler.Instruction{
		{Op: compiler.OP_SCAN, Args: []interface{}{"testdata/test_players"}},
		{Op: compiler.OP_LOAD_CONST, Args: []interface{}{true}},
		{Op: compiler.OP_OR}, // Only 1 operand on stack
		{Op: compiler.OP_OUTPUT},
	}

	_, err := Execute(instructions)

	if err == nil {
		t.Error("Expected error for OR stack underflow")
	}
	if err.Error() != "OR requires 2 boolean operands" {
		t.Errorf("Expected OR error, got: %s", err.Error())
	}
}

func TestExecute_FilterStackUnderflow(t *testing.T) {
	instructions := []compiler.Instruction{
		{Op: compiler.OP_SCAN, Args: []interface{}{"testdata/test_players"}},
		{Op: compiler.OP_FILTER}, // No condition on stack
		{Op: compiler.OP_OUTPUT},
	}

	_, err := Execute(instructions)

	if err == nil {
		t.Error("Expected error for FILTER stack underflow")
	}
	if err.Error() != "FILTER requires condition value" {
		t.Errorf("Expected FILTER error, got: %s", err.Error())
	}
}

func TestExecute_ProjectMissingField(t *testing.T) {
	instructions := []compiler.Instruction{
		{Op: compiler.OP_SCAN, Args: []interface{}{"testdata/test_players"}},
		{Op: compiler.OP_PROJECT, Args: []interface{}{"nonexistent_field"}},
		{Op: compiler.OP_OUTPUT},
	}

	_, err := Execute(instructions)

	if err == nil {
		t.Error("Expected error for projecting missing field")
	}
	if err.Error() != "Cannot project missing field 'nonexistent_field'" {
		t.Errorf("Expected project error, got: %s", err.Error())
	}
}

func TestExecute_LogicalOperations(t *testing.T) {
	instructions := []compiler.Instruction{
		{Op: compiler.OP_SCAN, Args: []interface{}{"testdata/test_players"}},
		// season = 2025
		{Op: compiler.OP_LOAD_FIELD, Args: []interface{}{"season"}},
		{Op: compiler.OP_LOAD_CONST, Args: []interface{}{"2025"}},
		{Op: compiler.OP_EQ},
		// team = TeamA
		{Op: compiler.OP_LOAD_FIELD, Args: []interface{}{"team"}},
		{Op: compiler.OP_LOAD_CONST, Args: []interface{}{"TeamA"}},
		{Op: compiler.OP_EQ},
		// AND them together
		{Op: compiler.OP_AND},
		{Op: compiler.OP_FILTER},
		{Op: compiler.OP_PROJECT, Args: []interface{}{"pts"}},
		{Op: compiler.OP_OUTPUT},
	}

	results, err := Execute(instructions)

	if err != nil {
		t.Fatalf("Expected no error, got: %v", err)
	}

	expectedCount := 1 // Only Player3 matches both conditions
	if len(results) != expectedCount {
		t.Errorf("Expected %d results, got %d", expectedCount, len(results))
	}
}

func TestExecute_OROperation(t *testing.T) {
	instructions := []compiler.Instruction{
		{Op: compiler.OP_SCAN, Args: []interface{}{"testdata/test_players"}},
		// season = 2024
		{Op: compiler.OP_LOAD_FIELD, Args: []interface{}{"season"}},
		{Op: compiler.OP_LOAD_CONST, Args: []interface{}{"2024"}},
		{Op: compiler.OP_EQ},
		// season = 2025  
		{Op: compiler.OP_LOAD_FIELD, Args: []interface{}{"season"}},
		{Op: compiler.OP_LOAD_CONST, Args: []interface{}{"2025"}},
		{Op: compiler.OP_EQ},
		// OR them together (should match all players)
		{Op: compiler.OP_OR},
		{Op: compiler.OP_FILTER},
		{Op: compiler.OP_PROJECT, Args: []interface{}{"player_id"}},
		{Op: compiler.OP_OUTPUT},
	}

	results, err := Execute(instructions)

	if err != nil {
		t.Fatalf("Expected no error, got: %v", err)
	}

	expectedCount := 4 // All players should match
	if len(results) != expectedCount {
		t.Errorf("Expected %d results, got %d", expectedCount, len(results))
	}
}

func TestExecute_NoFilter_AllRows(t *testing.T) {
	instructions := []compiler.Instruction{
		{Op: compiler.OP_SCAN, Args: []interface{}{"testdata/test_players"}},
		{Op: compiler.OP_PROJECT, Args: []interface{}{"player_id", "name"}},
		{Op: compiler.OP_OUTPUT},
	}

	results, err := Execute(instructions)

	if err != nil {
		t.Fatalf("Expected no error, got: %v", err)
	}

	expectedCount := 4 // All players
	if len(results) != expectedCount {
		t.Errorf("Expected %d results, got %d", expectedCount, len(results))
	}
}

func TestExecute_FilterFalse_NoResults(t *testing.T) {
	instructions := []compiler.Instruction{
		{Op: compiler.OP_SCAN, Args: []interface{}{"testdata/test_players"}},
		{Op: compiler.OP_LOAD_FIELD, Args: []interface{}{"season"}},
		{Op: compiler.OP_LOAD_CONST, Args: []interface{}{"2026"}}, // No matching season
		{Op: compiler.OP_EQ},
		{Op: compiler.OP_FILTER},
		{Op: compiler.OP_PROJECT, Args: []interface{}{"pts"}},
		{Op: compiler.OP_OUTPUT},
	}

	results, err := Execute(instructions)

	if err != nil {
		t.Fatalf("Expected no error, got: %v", err)
	}

	if len(results) != 0 {
		t.Errorf("Expected 0 results, got %d", len(results))
	}
}

func TestExecute_UnsupportedInstruction(t *testing.T) {
	instructions := []compiler.Instruction{
		{Op: compiler.OP_SCAN, Args: []interface{}{"testdata/test_players"}},
		{Op: -1, Args: []interface{}{}},
		{Op: compiler.OP_OUTPUT},
	}

	_, err := Execute(instructions)

	if err == nil {
		t.Error("Expected error for unsupported instruction")
	}
	if err.Error() != "unsupported instruction: UNSUPPORTED_OP" {
		t.Errorf("Expected unsupported instruction error, got: %s", err.Error())
	}
}

// Benchmark test for performance measurement
func BenchmarkExecute(b *testing.B) {
	instructions := []compiler.Instruction{
		{Op: compiler.OP_SCAN, Args: []interface{}{"testdata/test_players"}},
		{Op: compiler.OP_LOAD_FIELD, Args: []interface{}{"season"}},
		{Op: compiler.OP_LOAD_CONST, Args: []interface{}{"2025"}},
		{Op: compiler.OP_EQ},
		{Op: compiler.OP_FILTER},
		{Op: compiler.OP_PROJECT, Args: []interface{}{"pts", "date"}},
		{Op: compiler.OP_OUTPUT},
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		Execute(instructions)
	}
}