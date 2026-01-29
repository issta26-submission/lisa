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
//<ID> 111
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(123.0);
    cJSON *greet_item = cJSON_CreateString("hello");
    cJSON *replacement_greet = cJSON_CreateString("world");
    cJSON *found_num = NULL;
    cJSON *found_greet = NULL;
    char *printed = NULL;
    double num_val = 0.0;
    cJSON_bool added_num = 0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    added_num = cJSON_AddItemToObject(root, "num", num_item);
    cJSON_AddItemToObject(root, "greeting", greet_item);

    // step 3: Operate / Validate
    found_num = cJSON_GetObjectItem(root, "num");
    found_greet = cJSON_GetObjectItem(root, "greeting");
    num_val = cJSON_GetNumberValue(found_num);
    cJSON_AddNumberToObject(root, "num_times_two", num_val * 2.0);
    replaced = cJSON_ReplaceItemViaPointer(root, found_greet, replacement_greet);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}