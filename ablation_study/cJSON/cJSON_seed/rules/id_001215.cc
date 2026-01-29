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
//<ID> 1215
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *array = NULL;
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    cJSON *copy = NULL;
    cJSON *retrieved = NULL;
    char *printed = NULL;
    const char *retrieved_val = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    array = cJSON_CreateArray();
    item1 = cJSON_CreateString("alpha");
    item2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(array, item1);
    cJSON_AddItemToArray(array, item2);
    cJSON_AddItemToObject(root, "letters", array);
    cJSON_AddItemToObject(root, "single", cJSON_CreateString("one"));

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(array);
    retrieved = cJSON_GetArrayItem(array, 1);
    retrieved_val = cJSON_GetStringValue(retrieved);
    copy = cJSON_CreateString(retrieved_val);
    cJSON_AddItemToObject(root, "copy", copy);
    printed = cJSON_PrintUnformatted(root);
    (void)arr_size;
    (void)retrieved_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}