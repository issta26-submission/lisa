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
//<ID> 699
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
    char *printed_buf = (char *)cJSON_malloc(512);
    memset(printed_buf, 0, 512);
    const char *json = "{\"name\":\"example\",\"flag\":true,\"value\":123}";
    const char *end1 = NULL;
    const char *end2 = NULL;
    cJSON *parsed_with_length = NULL;
    cJSON *parsed_with_opts = NULL;
    cJSON *out = NULL;
    cJSON *name_item = NULL;
    cJSON *flag_item = NULL;
    const char *name_value = NULL;
    cJSON_bool name_is_string = 0;
    cJSON_bool flag_bool = 0;
    cJSON_bool printed_ok = 0;
    cJSON_bool out_name_is_string = 0;

    // step 2: Setup / Configure
    parsed_with_length = cJSON_ParseWithLengthOpts(json, strlen(json), &end1, 1);
    parsed_with_opts = cJSON_ParseWithOpts(json, &end2, 1);
    out = cJSON_CreateObject();
    name_item = cJSON_GetObjectItem(parsed_with_length, "name");
    flag_item = cJSON_GetObjectItem(parsed_with_length, "flag");
    name_value = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(out, "name_copy", name_value);
    flag_bool = cJSON_IsTrue(flag_item);
    cJSON_AddBoolToObject(out, "flag_copy", flag_bool);

    // step 3: Operate / Validate
    name_is_string = cJSON_IsString(name_item);
    printed_ok = cJSON_PrintPreallocated(out, printed_buf, 512, 1);
    out_name_is_string = cJSON_IsString(cJSON_GetObjectItem(out, "name_copy"));
    memcpy(scratch, printed_buf, strlen(printed_buf));
    scratch[strlen(printed_buf)] = '\0';

    // step 4: Cleanup
    cJSON_free(printed_buf);
    cJSON_free(scratch);
    cJSON_Delete(parsed_with_length);
    cJSON_Delete(parsed_with_opts);
    cJSON_Delete(out);

    // API sequence test completed successfully
    return 66;
}