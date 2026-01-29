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
//<ID> 17
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"id\":123,\"name\":\"fuzz\"}";
    cJSON *root = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");
    cJSON *bfalse = cJSON_CreateFalse();
    cJSON_AddItemToArray(flags, bfalse);

    // step 3: Operate and Validate
    int flags_size = cJSON_GetArraySize(flags);
    cJSON *first_flag = cJSON_GetArrayItem(flags, 0);
    cJSON_bool first_is_false = cJSON_IsFalse(first_flag);
    cJSON *id_item = cJSON_GetObjectItem(root, "id");
    double id_val = cJSON_GetNumberValue(id_item);
    int validation = (int)first_is_false + (int)(flags_size == 1) + (int)(id_val == 123.0);
    cJSON_SetNumberHelper(id_item, (double)validation);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}