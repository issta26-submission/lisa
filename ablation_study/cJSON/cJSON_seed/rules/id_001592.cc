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
//<ID> 1592
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *count = NULL;
    cJSON *flag = NULL;
    cJSON *nul = NULL;
    cJSON *tmp = NULL;
    char *out = NULL;
    double read_value = 0.0;
    cJSON_bool is_false = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    count = cJSON_CreateNumber(123.0);
    flag = cJSON_CreateFalse();
    nul = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "count", count);
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddItemToObject(root, "empty", nul);

    // step 3: Operate / Validate
    tmp = cJSON_GetObjectItem(root, "count");
    read_value = cJSON_GetNumberValue(tmp);
    is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "flag"));
    out = cJSON_PrintUnformatted(root);
    cJSON_DeleteItemFromObject(root, "empty");
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}