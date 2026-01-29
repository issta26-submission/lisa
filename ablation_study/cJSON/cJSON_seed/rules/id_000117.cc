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
//<ID> 117
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_orig = cJSON_CreateNumber(3.1415);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON *found_num = NULL;
    cJSON *replacement_num = NULL;
    char *printed = NULL;
    double value = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "pi", num_orig);
    cJSON_AddItemToObject(root, "greeting", str_item);

    // step 3: Operate / Validate
    found_num = cJSON_GetObjectItem(root, "pi");
    value = cJSON_GetNumberValue(found_num);
    replacement_num = cJSON_CreateNumber(value * 2.0);
    cJSON_ReplaceItemViaPointer(root, found_num, replacement_num);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}