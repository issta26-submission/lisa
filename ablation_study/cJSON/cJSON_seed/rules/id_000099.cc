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
//<ID> 99
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *mirror = cJSON_CreateObject();
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(7.0);
    cJSON *raw = cJSON_CreateRaw("true");
    cJSON *m_n1 = NULL;
    cJSON *m_n2 = NULL;
    cJSON *m_raw = NULL;
    char *printed = NULL;
    double v1 = 0.0;
    double v2 = 0.0;
    double sum = 0.0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "n1", num1);
    cJSON_AddItemToObject(root, "n2", num2);
    cJSON_AddItemToObject(root, "r", raw);
    m_n1 = cJSON_CreateNumber(42.0);
    m_n2 = cJSON_CreateNumber(7.0);
    m_raw = cJSON_CreateRaw("true");
    cJSON_AddItemToObject(mirror, "n1", m_n1);
    cJSON_AddItemToObject(mirror, "n2", m_n2);
    cJSON_AddItemToObject(mirror, "r", m_raw);

    // step 3: Operate / Validate
    v1 = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "n1"));
    v2 = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "n2"));
    sum = v1 + v2;
    cJSON_AddNumberToObject(root, "sum", sum);
    equal = cJSON_Compare(root, mirror, 1);
    cJSON_AddBoolToObject(root, "equal", equal);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(mirror);

    // API sequence test completed successfully
    return 66;
}