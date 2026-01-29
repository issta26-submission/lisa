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
//<ID> 1106
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *array = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *fetched0 = (cJSON *)0;
    cJSON *fetched_data = (cJSON *)0;
    cJSON *fetched_name = (cJSON *)0;
    int validation_score = 0;

    // step 2: Setup - create an object root, an array, a string and a true boolean
    root = cJSON_CreateObject();
    array = cJSON_CreateArray();
    str_item = cJSON_CreateString("first_element");
    true_item = cJSON_CreateTrue();

    // step 3: Configure - attach the string and boolean to the array, then attach array and a name string to root
    cJSON_AddItemToArray(array, str_item);
    cJSON_AddItemToArray(array, true_item);
    cJSON_AddItemToObject(root, "data", array);
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("container"));

    // step 4: Operate - fetch items from the array and object using the required getters
    fetched0 = cJSON_GetArrayItem(array, 0);
    fetched_data = cJSON_GetObjectItem(root, "data");
    fetched_name = cJSON_GetObjectItem(root, "name");

    // step 5: Validate - compute a small validation score based on non-null pointers
    validation_score = (int)(root != (cJSON *)0) + (int)(array != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(true_item != (cJSON *)0) + (int)(fetched0 != (cJSON *)0) + (int)(fetched_data != (cJSON *)0) + (int)(fetched_name != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}