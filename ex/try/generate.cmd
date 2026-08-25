HOW TO GENERATE:
   SHARE followers, starters, enders
   PUT choice starters IN c1, c2
   WRITE c1^c2
   WHILE c2 not.in enders:
      PUT choice followers[c1, c2] IN c3
      WRITE c3
      PUT c2, c3 IN c1, c2
   WRITE /
