//go:build enable_tracy

package tracy

import "testing"

func TestToCString_ReturnsSameResultForSameString(t *testing.T) {
	strA := "example string A"
	strB := "example string B"

	a := toCString(strA)
	b := toCString(strB)
	c := toCString(strA)

	if a == b {
		t.Errorf("toCString(%q) == toCString(%q), expected different results", strA, strB)
	}

	if a != c {
		t.Errorf("toCString(%q) != toCString(%q), expected same result", strA, strA)
	}
}
