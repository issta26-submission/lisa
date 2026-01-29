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
//<ID> 786
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "[1, 2, 3]";
    cJSON *parsed = cJSON_Parse(json);
    cJSON *root = cJSON_CreateObject();
    cJSON *array_in_root = NULL;
    cJSON *new_num = cJSON_CreateNumber(42.5);
    char *printed = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "numbers", parsed);
    array_in_root = cJSON_GetObjectItem(root, "numbers");

    // step 3: Operate / Validate
    cJSON_ReplaceItemInArray(array_in_root, 1, new_num);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}