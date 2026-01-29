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
//<ID> 1596
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *null_item = NULL;
    cJSON *flag_item = NULL;
    cJSON_bool flag_is_false = 0;
    char *serialized = NULL;
    double extracted_num = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);
    null_item = cJSON_CreateNull();
    flag_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "num", num_item);
    cJSON_AddItemToObject(root, "empty", null_item);
    cJSON_AddItemToObject(root, "flag", flag_item);

    // step 3: Operate / Validate
    serialized = cJSON_PrintUnformatted(root);
    extracted_num = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "num"));
    flag_is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "flag"));
    cJSON_DeleteItemFromObject(root, "num");

    // step 4: Cleanup
    cJSON_free(serialized);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}