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
//<ID> 725
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
    char *printed = NULL;
    size_t printed_len = 0;
    cJSON *root = cJSON_CreateObject();
    cJSON *obj = cJSON_CreateObject();
    cJSON *flag_old = cJSON_CreateTrue();
    cJSON *replacement_true = cJSON_CreateTrue();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *got_flag = NULL;
    double got_value = 0.0;
    cJSON_bool replaced1 = 0;
    cJSON_bool replaced2 = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "obj", obj);
    cJSON_AddItemToObject(obj, "flag", flag_old);
    replaced1 = cJSON_ReplaceItemInObjectCaseSensitive(obj, "flag", replacement_true);
    replaced2 = cJSON_ReplaceItemViaPointer(obj, replacement_true, num_item);

    // step 3: Operate / Validate
    got_flag = cJSON_GetObjectItem(obj, "flag");
    got_value = cJSON_GetNumberValue(got_flag);
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