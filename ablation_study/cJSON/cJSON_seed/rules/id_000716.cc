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
//<ID> 716
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = NULL;
    cJSON *num_root = NULL;
    cJSON *num_nested = NULL;
    cJSON *flag = NULL;
    double root_val = 0.0;
    double nested_val = 0.0;
    double computed = 0.0;
    cJSON_bool active_true = 0;
    char *printed = NULL;
    size_t printed_len = 0;

    // step 2: Setup / Configure
    num_root = cJSON_AddNumberToObject(root, "value", 42.5);
    nested = cJSON_AddObjectToObject(root, "meta");
    num_nested = cJSON_AddNumberToObject(nested, "factor", 2.0);
    flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", flag);

    // step 3: Operate / Validate
    root_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    nested_val = cJSON_GetNumberValue(cJSON_GetObjectItem(nested, "factor"));
    computed = root_val * nested_val;
    cJSON_AddNumberToObject(root, "computed", computed);
    active_true = cJSON_IsTrue(cJSON_GetObjectItem(root, "active"));
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    memcpy(scratch, printed ? printed : "", printed_len);
    scratch[printed_len] = '\0';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}