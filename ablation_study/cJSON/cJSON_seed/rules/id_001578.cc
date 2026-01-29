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
//<ID> 1578
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root_parsed = NULL;
    cJSON *root_created = NULL;
    cJSON *item_parsed = NULL;
    cJSON *item_created = NULL;
    char *printed = NULL;
    const char *json_text = "{\"value\":3.14}";
    cJSON_bool cmp = 0;
    double parsed_value = 0.0;

    // step 2: Setup / Configure
    root_parsed = cJSON_Parse(json_text);
    root_created = cJSON_CreateObject();
    cJSON_AddNumberToObject(root_created, "value", 3.14);

    // step 3: Operate / Validate
    item_parsed = cJSON_GetObjectItem(root_parsed, "value");
    item_created = cJSON_GetObjectItem(root_created, "value");
    parsed_value = cJSON_GetNumberValue(item_parsed);
    printed = cJSON_Print(root_parsed);
    cmp = cJSON_Compare(root_parsed, root_created, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root_parsed);
    cJSON_Delete(root_created);
    // API sequence test completed successfully
    return 66;
}