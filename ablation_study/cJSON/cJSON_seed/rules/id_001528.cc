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
//<ID> 1528
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *values = NULL;
    cJSON *first = NULL;
    cJSON *name = NULL;
    const char *version = NULL;
    const char *name_val = NULL;
    double first_num = 0.0;
    int values_size = 0;
    char json[] = "{ \"name\": \"TestUser\", \"values\": [10, 20, 30] }";
    char buffer[32];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    parsed = cJSON_Parse(json);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);

    // step 3: Operate / Validate
    version = cJSON_Version();
    values = cJSON_GetObjectItem(parsed, "values");
    values_size = cJSON_GetArraySize(values);
    first = cJSON_GetArrayItem(values, 0);
    first_num = cJSON_GetNumberValue(first);
    name = cJSON_GetObjectItem(parsed, "name");
    name_val = cJSON_GetStringValue(name);
    buffer[0] = version[0];
    buffer[1] = name_val[0];
    buffer[2] = '\0';
    (void)values_size;
    (void)first_num;
    (void)buffer;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}