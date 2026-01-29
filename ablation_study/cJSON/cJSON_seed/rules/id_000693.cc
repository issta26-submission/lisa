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
//<ID> 693
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    char *printed_buf = (char *)cJSON_malloc(256);
    memset(printed_buf, 0, 256);
    const char *json = "{\"name\":\"Alice\",\"active\":true,\"age\":30}";
    size_t json_len = strlen(json);
    const char *end1 = NULL;
    const char *end2 = NULL;
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed1 = cJSON_ParseWithLengthOpts(json, json_len, &end1, 1);
    cJSON *parsed2 = cJSON_ParseWithOpts(json, &end2, 1);
    cJSON *name_item = cJSON_GetObjectItem(parsed2, "name");
    cJSON_bool name_is_str = cJSON_IsString(name_item);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "original_parse", parsed1);
    cJSON_AddItemToObject(root, "second_parse", parsed2);
    cJSON *flag = cJSON_AddBoolToObject(root, "name_is_string", name_is_str);
    cJSON_AddBoolToObject(root, "explicit_true", 1);

    // step 3: Operate / Validate
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, printed_buf, 256, 0);
    size_t printed_len = strlen(printed_buf);
    size_t to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(scratch, printed_buf, to_copy);
    scratch[to_copy] = '\0';

    // step 4: Cleanup
    cJSON_free(printed_buf);
    cJSON_Delete(root);
    cJSON_free(scratch);

    // API sequence test completed successfully
    return 66;
}