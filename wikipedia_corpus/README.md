# WIKIPEDIA CORPUS

## MOTIVATION

Constructing corpus with special theme through Wikipedia.

## USAGE

Just scan the code and guess how to use it. It's simple! 

Or you are patient...

### Step 1 - get categories with special theme

To get corpus with special theme, you need to choose some related keywords first to kick off the explore of categories. For example, if I wanna corpus with theme 'computer', keyworkds 'Computers', 'Computer_science', and 'Machine_learning' can be good choices. Note that please ensure your keywords are avaliable wikipedia categories (check by accessing https://en.wikipedia.org/wiki/Category:{Your_keywords}).

Now that finish deciding keywords, fill them in extract_categories.py and choose a reasonable value for level(1 is recommand-ed). You can get some categories related to your special themes. Don't forget to remove proxies in requests.get if you don't live in China mainland.

### Step 2 - download dump file of wikipedia and extract related pages' content

Download [enwiki-latest-pages-articles.xml.bz2](https://dumps.wikimedia.org/enwiki/latest/) and decompress it. Run wiki.py with right arguments setup which is described in source file. 
