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
//<ID> 712
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
    cJSON *outer = NULL;
    cJSON *inner = NULL;
    cJSON *num_item = NULL;
    cJSON *flag_item = NULL;
    cJSON_bool flag_is_true = 0;
    double num_value = 0.0;
    char *printed = NULL;
    size_t printed_len = 0;

    // step 2: Setup / Configure
    outer = cJSON_AddObjectToObject(root, "outer");
    inner = cJSON_AddObjectToObject(outer, "inner");
    cJSON_AddNumberToObject(inner, "num", 42.0);
    cJSON_AddTrueToObject(outer, "flag");

    // step 3: Operate / Validate
    num_item = cJSON_GetObjectItem(inner, "num");
    num_value = cJSON_GetNumberValue(num_item);
    flag_item = cJSON_GetObjectItem(outer, "flag");
    flag_is_true = cJSON_IsTrue(flag_item);
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