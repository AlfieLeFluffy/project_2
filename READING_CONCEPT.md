# project_2

set_t *set;
rel_t *rel;

//...

int line = 0;
//pro prvni radek prirazeni U

line++;
while (/*radek neni prazdny*/)
{
  if (/*prvni char radku == S*/)
  {
    //zvetsit set
    //priradit line (neco jako set[/*posledni prvek*/].line = line)
    
    //pro kazdy podretezec radku (prvek)
      //pridat prvek do mnoziny (neco jako set[/*posledni prvek*/].elements[i] = /*podretezec*/)
  }
  else if (/*prvni char radku == R*/)
  {
    //zvetsit rel
    //priradit line (neco jako rel[/*posledni prvek*/].line = line)
    
    //pro kazdy podretezec radku, zacinajici '(' (dvojce prvku)
      //pro prvni podretezec podretezce (prvni prvek)
        //pridat prvni prvek do relace (neco jako rel[/*posledni prvek*/].elements[i].el1 = /*podretezec*/)
      //pro druhy podretezec podretezce (druhy prvek)
        //pridat druhy prvek do relace (neco jako rel[/*posledni prvek*/].elements[i].el2 = /*podretezec*/)
  }

  line++;
}

//...
