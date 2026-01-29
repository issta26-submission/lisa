#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1052
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *words[] = { "alpha", "beta", "gamma" };
    cJSON *root = (cJSON *)0;
    cJSON *str_array = (cJSON *)0;
    cJSON *fetched_array = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    char *printed = (char *)0;
    char *val0 = (char *)0;
    char *val1 = (char *)0;
    int array_size = 0;
    cJSON_bool add_ok = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and a string array, then attach array to root
    root = cJSON_CreateObject();
    str_array = cJSON_CreateStringArray(words, 3);
    add_ok = cJSON_AddItemToObject(root, "words", str_array);

    // step 3: Operate - retrieve the array and its elements, produce an unformatted print and free it
    fetched_array = cJSON_GetObjectItem(root, "words");
    item0 = cJSON_GetArrayItem(fetched_array, 0);
    item1 = cJSON_GetArrayItem(fetched_array, 1);
    val0 = cJSON_GetStringValue(item0);
    val1 = cJSON_GetStringValue(item1);
    array_size = cJSON_GetArraySize(fetched_array);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 4: Validate - compute a small validation score using the results
    validation_score = (int)(root != (cJSON *)0) + (int)(str_array != (cJSON *)0) + (int)(add_ok) + (int)(val0 != (char *)0) + (int)(val1 != (char *)0) + (int)(array_size == 3);
    (void)validation_score;

    // step 5: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}