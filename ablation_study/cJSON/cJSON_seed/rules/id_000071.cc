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
//<ID> 71
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON *num_item = NULL;
    cJSON *found_num = NULL;
    cJSON *found_str = NULL;
    char *printed = NULL;
    char *str_val = NULL;
    double num_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "message", str_item);
    num_item = cJSON_AddNumberToObject(root, "initial", 42.0);

    // step 3: Operate / Validate
    found_num = cJSON_GetObjectItem(root, "initial");
    num_val = cJSON_GetNumberValue(found_num);
    found_str = cJSON_GetObjectItem(root, "message");
    str_val = cJSON_GetStringValue(found_str);
    cJSON_AddNumberToObject(root, "total", num_val + 3.0);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}