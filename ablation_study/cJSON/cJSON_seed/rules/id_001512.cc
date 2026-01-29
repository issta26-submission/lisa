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
//<ID> 1512
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *name_item = NULL;
    cJSON *nested = NULL;
    cJSON *age_item = NULL;
    cJSON *ref_name = NULL;
    cJSON *ref_parseend = NULL;
    const char *parse_end = NULL;
    const char *name_value = NULL;
    double age_value = 0.0;
    char json[] = "{\"name\":\"Bob\",\"info\":{\"age\":30},\"list\":[\"a\",\"b\"]}";

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithOpts(json, &parse_end, 1);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    ref_name = cJSON_CreateStringReference("reference hello");
    cJSON_AddItemToObject(root, "ref", ref_name);

    // step 3: Operate / Validate
    name_item = cJSON_GetObjectItem(parsed, "name");
    name_value = cJSON_GetStringValue(name_item);
    nested = cJSON_GetObjectItem(parsed, "info");
    age_item = cJSON_GetObjectItem(nested, "age");
    age_value = cJSON_GetNumberValue(age_item);
    cJSON_AddStringToObject(root, "name_copy", name_value);
    cJSON_AddNumberToObject(root, "age_copy", age_value);
    ref_parseend = cJSON_CreateStringReference(parse_end);
    cJSON_AddItemToObject(root, "parse_end_ref", ref_parseend);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}