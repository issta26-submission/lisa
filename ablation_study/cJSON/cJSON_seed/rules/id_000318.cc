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
//<ID> 318
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_CreateObject();
    cJSON *flags = cJSON_CreateObject();
    cJSON *added_bool = NULL;
    cJSON *added_array = NULL;
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON_bool is_false = 0;
    cJSON_bool is_invalid = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "config", cfg);
    cJSON_AddItemToObject(cfg, "flags", flags);
    added_bool = cJSON_AddBoolToObject(flags, "enabled", 0);
    added_array = cJSON_AddArrayToObject(cfg, "numbers");
    cJSON_AddItemToArray(added_array, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(added_array, cJSON_CreateNumber(20.0));

    // step 3: Operate / Validate
    is_false = cJSON_IsFalse(added_bool);
    is_invalid = cJSON_IsInvalid(added_array);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = (char)('0' + (int)is_false);
    buffer[1] = (char)('A' + (int)is_invalid);
    buffer[2] = printed[0];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}