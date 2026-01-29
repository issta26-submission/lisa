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
//<ID> 578
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    const char *static_str = "fuzz_ref_string";
    cJSON *str_ref = cJSON_CreateStringReference(static_str);
    cJSON *num_item = cJSON_CreateNumber(1234.0);
    cJSON *flag_item = cJSON_CreateTrue();
    char *printed = NULL;
    size_t printed_len = 0;
    size_t to_copy = 0;
    cJSON *detached = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "data", array);
    cJSON_AddItemReferenceToArray(array, str_ref);
    cJSON_AddItemToArray(array, num_item);
    cJSON_AddItemToObject(root, "enabled", flag_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(buffer, printed ? printed : "", to_copy);
    buffer[to_copy] = '\0';
    detached = cJSON_DetachItemFromObject(root, "data");
    cJSON_free(printed);
    printed = cJSON_PrintUnformatted(detached);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(buffer, printed ? printed : "", to_copy);
    buffer[to_copy] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}