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
//<ID> 762
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_buf[] = " { \"alpha\" : \" one \" , \"beta\" : false } ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON *child_flag = cJSON_CreateTrue();
    cJSON_AddItemToObjectCS(child, "child_flag", child_flag);
    cJSON_AddItemToObjectCS(root, "parsed_data", parsed);
    cJSON_AddItemToObjectCS(root, "child", child);
    cJSON *explicit_bool = cJSON_AddBoolToObject(root, "explicit_flag", 1);
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObjectCS(root, "child_reference", child_ref);

    // step 3: Operate
    char *printed = cJSON_Print(root);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    (void)explicit_bool;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}