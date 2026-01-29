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
//<ID> 1107
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *name = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *arr_elem0 = (cJSON *)0;
    cJSON *arr_elem1 = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_name = (cJSON *)0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object, an array, a name string and a true boolean
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    name = cJSON_CreateString("device_name");
    true_item = cJSON_CreateTrue();

    // step 3: Configure - populate array with two string items and attach items to root
    arr_elem0 = cJSON_CreateString("entry0");
    arr_elem1 = cJSON_CreateString("entry1");
    cJSON_AddItemToArray(arr, arr_elem0);
    cJSON_AddItemToArray(arr, arr_elem1);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "active", true_item);

    // step 4: Operate - fetch the array and name from the root, then fetch an element from the array
    fetched_arr = cJSON_GetObjectItem(root, "data");
    fetched_name = cJSON_GetObjectItem(root, "name");
    cJSON *second_element = cJSON_GetArrayItem(fetched_arr, 1);
    (void)second_element;

    // step 5: Validate - serialize and compute a small validation score
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(name != (cJSON *)0) + (int)(true_item != (cJSON *)0) + (int)(fetched_arr != (cJSON *)0) + (int)(fetched_name != (cJSON *)0) + (int)(second_element != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}