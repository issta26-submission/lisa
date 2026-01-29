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
//<ID> 4
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *text = cJSON_CreateString("example");
    cJSON *flag = cJSON_CreateTrue();
    cJSON *raw = cJSON_CreateRaw("{\"embedded\":true}");

    // step 2: Configure (attach items to root)
    cJSON_AddItemToObject(root, "number", num);
    cJSON_AddItemToObject(root, "text", text);
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddItemToObject(root, "embedded_raw", raw);
    cJSON_AddNullToObject(root, "nothing");

    // step 3: Operate / Validate (produce strings, inspect types, modify number)
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_Minify(unformatted);
    char *pretty = cJSON_Print(root);
    cJSON_SetNumberHelper(num, 84.0);
    double current_number = cJSON_GetNumberValue(num);
    cJSON *got_flag = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_is_true = cJSON_IsTrue(got_flag);
    cJSON *got_raw = cJSON_GetObjectItem(root, "embedded_raw");
    cJSON_bool raw_is_raw = cJSON_IsRaw(got_raw);

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_free(pretty);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)current_number;
    (void)flag_is_true;
    (void)raw_is_raw;
    return 66;
}