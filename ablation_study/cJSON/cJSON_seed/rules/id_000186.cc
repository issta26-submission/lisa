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
//<ID> 186
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *str_item = cJSON_CreateString("example");
    const char *version_ptr = cJSON_Version();
    void *storage = cJSON_malloc(64);
    memset(storage, 0, 64);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "number", num_item);
    cJSON_AddItemToObject(root, "label", str_item);
    /* store the version pointer into allocated storage for later retrieval */
    *((const char **)storage) = version_ptr;

    // step 3: Operate / Validate
    cJSON *retrieved_num = cJSON_GetObjectItem(root, "number");
    double retrieved_value = cJSON_GetNumberValue(retrieved_num);
    cJSON *retrieved_str = cJSON_GetObjectItem(root, "label");
    cJSON_bool retrieved_is_string = cJSON_IsString(retrieved_str);
    const char *retrieved_version = *((const char **)storage);
    (void)retrieved_value;
    (void)retrieved_is_string;
    (void)retrieved_version;

    // step 4: Cleanup
    cJSON_free(storage);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}