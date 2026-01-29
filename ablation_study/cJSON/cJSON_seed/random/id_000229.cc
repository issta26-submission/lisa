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
//<ID> 229
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char json_text[] = "{\"flag\":true,\"value\":42.5}";
    const size_t json_len = (sizeof(json_text) - 1);
    const char *parse_end = (const char *)0;
    cJSON *root = (cJSON *)0;
    cJSON *value_item = (cJSON *)0;
    char *value_printed = (char *)0;
    char *final_text = (char *)0;
    cJSON_bool is_object = (cJSON_bool)0;
    double numeric_value = 0.0;

    // step 2: Initialize (parse with explicit length and null-termination requirement)
    root = cJSON_ParseWithLengthOpts(json_text, json_len, &parse_end, (cJSON_bool)1);

    // step 3: Configure (extract value and inspect type)
    value_item = cJSON_GetObjectItem(root, "value");
    numeric_value = cJSON_GetNumberValue(value_item);
    is_object = cJSON_IsObject(root);
    cJSON_AddBoolToObject(root, "is_object", is_object);

    // step 4: Operate (stringify the numeric item and add string and numeric copies)
    value_printed = cJSON_PrintUnformatted(value_item);
    cJSON_AddStringToObject(root, "value_as_string", value_printed);
    cJSON_AddNumberToObject(root, "value_copy", numeric_value);
    cJSON_AddStringToObject(root, "status", "parsed");
    cJSON_free(value_printed);

    // step 5: Validate (produce final JSON and free)
    final_text = cJSON_PrintUnformatted(root);
    cJSON_free(final_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}