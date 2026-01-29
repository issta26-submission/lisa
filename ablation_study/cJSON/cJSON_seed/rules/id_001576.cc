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
//<ID> 1576
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json_text1 = "{\"value\": 123}";
    const char *json_text2 = "{\"value\": 123.0}";
    cJSON *root1 = NULL;
    cJSON *root2 = NULL;
    cJSON *root3 = NULL;
    cJSON *val1 = NULL;
    cJSON *val2 = NULL;
    cJSON *val3 = NULL;
    char *printed = NULL;
    char *copybuf = NULL;
    size_t len = 0;
    double num1 = 0.0;
    double num2 = 0.0;
    double num3 = 0.0;
    cJSON_bool cmp12 = 0;
    cJSON_bool cmp13 = 0;

    // step 2: Setup / Configure
    root1 = cJSON_Parse(json_text1);
    root2 = cJSON_Parse(json_text2);

    // step 3: Operate / Validate
    val1 = cJSON_GetObjectItem(root1, "value");
    val2 = cJSON_GetObjectItem(root2, "value");
    num1 = cJSON_GetNumberValue(val1);
    num2 = cJSON_GetNumberValue(val2);
    cmp12 = cJSON_Compare(root1, root2, 1);
    printed = cJSON_PrintUnformatted(root1);
    len = strlen(printed) + 1;
    copybuf = (char *)cJSON_malloc(len);
    memset(copybuf, 0, len);
    memcpy(copybuf, printed, len);
    root3 = cJSON_Parse(copybuf);
    val3 = cJSON_GetObjectItem(root3, "value");
    num3 = cJSON_GetNumberValue(val3);
    cmp13 = cJSON_Compare(root1, root3, 1);

    // step 4: Cleanup
    cJSON_free(copybuf);
    cJSON_free(printed);
    cJSON_Delete(root1);
    cJSON_Delete(root2);
    cJSON_Delete(root3);
    // API sequence test completed successfully
    return 66;
}