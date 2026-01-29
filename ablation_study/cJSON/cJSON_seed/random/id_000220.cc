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
//<ID> 220
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char json[] = "{\"value\": 123.456, \"name\":\"sample\"}";
    size_t buf_len = sizeof(json) - 1;
    const char *parse_end = (const char *)0;
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON_bool is_obj = (cJSON_bool)0;
    double extracted_value = 0.0;

    // step 2: Initialize (parse with explicit length and opts)
    root = cJSON_ParseWithLengthOpts(json, buf_len, &parse_end, (cJSON_bool)1);

    // step 3: Configure (record that the parsed root is an object and attach original JSON)
    is_obj = cJSON_IsObject(root);
    cJSON_AddBoolToObject(root, "parsed_is_object", is_obj);
    cJSON_AddStringToObject(root, "original_json", json);

    // step 4: Operate (retrieve numeric field and extract its numeric value)
    num_item = cJSON_GetObjectItem(root, "value");
    extracted_value = cJSON_GetNumberValue(num_item);

    // step 5: Validate (re-add the extracted numeric value and a descriptor string)
    cJSON_AddNumberToObject(root, "reencoded_value", extracted_value);
    cJSON_AddStringToObject(root, "value_note", "extracted_and_reencoded");

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}