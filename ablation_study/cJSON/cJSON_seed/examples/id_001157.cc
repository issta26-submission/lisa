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
//<ID> 1157
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"test\",\"active\":true}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *created = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("test");
    cJSON *active_item = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_AddItemToObject(created, "name", name_item);
    cJSON_AddItemToObject(created, "active", active_item);

    // step 3: Operate & Validate
    cJSON_bool are_equal = cJSON_Compare(parsed, created, 1);
    cJSON *comparison_flag = cJSON_CreateBool(are_equal);
    cJSON_AddItemToObject(created, "equal_to_parsed", comparison_flag);
    char *out = cJSON_PrintUnformatted(created);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(created);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}