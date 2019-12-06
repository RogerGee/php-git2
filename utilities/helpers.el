;; Emacs utility functions for php-git2

(defun php-git2-make-constants ()
  "Translate C-language strings into php-git2 constant macro expressions"
  (interactive nil)
  (let ((case-fold-search nil)
        (text (buffer-substring-no-properties (region-beginning) (region-end)))
        (pos 0)
        (result ""))
    (while (string-match "\\(GIT_[A-Z0-9_]+\\)" text pos)
      (setq result (concat result (concat
                      "PHP_GIT2_CONSTANT("
                      (concat
                       (match-string 1 text)
                       ");\n"))))
      (setq pos (match-end 0))
      )
    (delete-region (region-beginning) (region-end))
    (insert result)
    )
  )

(defun php-git2-make-funcargs ()
  "Translate line-oriented function args from doc webpage into template args"
  (interactive nil)
  (let ((text (buffer-substring-no-properties (region-beginning) (region-end)))
        (pos 0)
        (result "int,\n"))
    (while (string-match "\\(const\\)? *\\([^][ *]+\\) *\\([*]+\\|[[][]]\\)?.*\n*" text pos)
      (if (match-string 1 text)
          (setq result (concat result "const ")))
      (setq result (concat result (match-string 2 text)))
      (if (match-string 3 text)
          (setq result (concat result (match-string 3 text))))
      (setq result (concat result ",\n"))
      (setq pos (match-end 0))
      )
    (delete-region (region-beginning) (region-end))
    (insert (substring result 0 (- (length result) 2)))
    )
  )
