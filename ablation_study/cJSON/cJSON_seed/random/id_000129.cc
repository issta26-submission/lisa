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
//<ID> 129
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *s_name = (cJSON *)0;
    cJSON *s_model_added = (cJSON *)0;
    cJSON *samples = (cJSON *)0;
    float sample_vals[3];
    double count_val = 0.0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    s_name = cJSON_CreateString("payload_sensor");
    sample_vals[0] = 0.25f;
    sample_vals[1] = -1.5f;
    sample_vals[2] = 3.1415f;

    // step 3: Configure
    cJSON_AddItemToObject(root, "name", s_name);
    s_model_added = cJSON_AddStringToObject(root, "model", "mK-2000");
    samples = cJSON_CreateFloatArray(sample_vals, 3);
    cJSON_AddItemToObject(root, "readings", samples);
    cJSON_AddNumberToObject(root, "count", 3.0);

    // step 4: Operate
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 5: Validate
    count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    cJSON_AddNumberToObject(root, "validated_count", count_val);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}