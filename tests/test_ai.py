import sys
import os
sys.path.append(os.path.join(os.path.dirname(__file__), '../ai_module'))

from translator import translate_natural_language

def test_translation():
    suggestions = translate_natural_language("show large files")
    assert len(suggestions) == 3
    assert "ls -lh" in [s["command"] for s in suggestions]

def test_correction():
    from corrector import correct_command
    corrected = correct_command("git psuh")
    assert "push" in corrected
