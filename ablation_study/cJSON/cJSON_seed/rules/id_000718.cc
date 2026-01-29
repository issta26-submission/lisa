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
//<ID> 718
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    char *printed_buf = (char *)cJSON_malloc(512);
    memset(printed_buf, 0, 512);
    const char *json_text = "{\"alpha\":1,\"beta\":2}";
    cJSON *root = cJSON_CreateObject();
    cJSON *child = NULL;
    cJSON *num_item = NULL;
    cJSON *flag_item = NULL;
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON_bool flag_true = 0;
    double num_value = 0.0;
    cJSON_bool printed_ok = 0;

    // step 2: Setup / Configure
    child = cJSON_AddObjectToObject(root, "nested");
    cJSON_AddNumberToObject(child, "num", 42.5);
    cJSON_AddTrueToObject(root, "flag");
    num_item = cJSON_GetObjectItem(child, "num");
    flag_item = cJSON_GetObjectItem(root, "flag");

    // step 3: Operate / Validate
    num_value = cJSON_GetNumberValue(num_item);
    flag_true = cJSON_IsTrue(flag_item);
    printed_ok = cJSON_PrintPreallocated(root, printed_buf, 512, 1);
    memcpy(scratch, printed_buf, strlen(printed_buf));
    scratch[strlen(printed_buf)] = '\0';

    // step 4: Cleanup
    cJSON_free(printed_buf);
    cJSON_free(scratch);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}